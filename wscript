#! /usr/bin/python3
# encoding: utf-8
#
# Copyright (C) 2015-2018 Karl Linden <karl.j.linden@gmail.com>
# Copyleft (C) 2008-2022 Nedko Arnaudov
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

from __future__ import print_function

import os
import shutil
import sys

from waflib import Logs, Options, TaskGen
from waflib import Context
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext
from waftoolchainflags import WafToolchainFlags

VERSION = "2.22.1"
APPNAME = 'jackdbus'

# these variables are mandatory ('/' are converted automatically)
top = '.'
out = 'build'

def display_feature(conf, msg, build):
    if build:
        conf.msg(msg, 'yes', color='GREEN')
    else:
        conf.msg(msg, 'no', color='YELLOW')


def options(opt):
    # options provided by the modules
    opt.load('compiler_c')
    opt.load('autooptions')

    # install directories
    opt.add_option(
        '--htmldir',
        type='string',
        default=None,
        help='HTML documentation directory [Default: <prefix>/share/jack-audio-connection-kit/reference/html/',
    )
    opt.add_option('--libdir', type='string', help='Library directory [Default: <prefix>/lib]')
    opt.add_option('--pkgconfigdir', type='string', help='pkg-config file directory [Default: <libdir>/pkgconfig]')
    opt.add_option('--mandir', type='string', help='Manpage directory [Default: <prefix>/share/man/man1]')

    # options affecting binaries
    opt.add_option(
        '--platform',
        type='string',
        default=sys.platform,
        help='Target platform for cross-compiling, e.g. cygwin or win32',
    )
    opt.add_option('--debug', action='store_true', default=False, dest='debug', help='Build debuggable binaries')
    opt.add_option('--siginfo', action='store_true', default=False, dest='siginfo', help="Log backtrace on fatal signal")

    #opt.set_auto_options_define('HAVE_%s')
    #opt.set_auto_options_style('yesno_and_hack')

    # options with third party dependencies
    #doxygen = opt.add_auto_option(
    #        'doxygen',
    #        help='Build doxygen documentation',
    #        conf_dest='BUILD_DOXYGEN_DOCS',
    #        default=False)
    #doxygen.find_program('doxygen')

    # dbus options
    opt.recurse('dbus')

    # this must be called before the configure phase
    #opt.apply_auto_options_hack()


def detect_platform(conf):
    # GNU/kFreeBSD and GNU/Hurd are treated as Linux
    platforms = [
        # ('KEY, 'Human readable name', ['strings', 'to', 'check', 'for'])
        ('IS_LINUX',   'Linux',   ['gnu0', 'gnukfreebsd', 'linux', 'posix']),
        ('IS_FREEBSD', 'FreeBSD', ['freebsd']),
        ('IS_MACOSX',  'MacOS X', ['darwin']),
        ('IS_SUN',     'SunOS',   ['sunos']),
        ('IS_WINDOWS', 'Windows', ['cygwin', 'msys', 'win32'])
    ]

    for key, name, strings in platforms:
        conf.env[key] = False

    conf.start_msg('Checking platform')
    platform = Options.options.platform
    for key, name, strings in platforms:
        for s in strings:
            if platform.startswith(s):
                conf.env[key] = True
                conf.end_msg(name, color='CYAN')
                break

def configure(conf):
    conf.load('compiler_c')

    detect_platform(conf)
    flags = WafToolchainFlags(conf)

    conf.check_cfg(package='jackserver', uselib_store='JACKSERVER', args=["--cflags", "--libs"])

    conf.check_cfg(package='expat', args='--cflags --libs')

    flags.add_c('-Wall')

    if conf.env['IS_FREEBSD']:
        conf.check(lib='execinfo', uselib='EXECINFO', define_name='EXECINFO')
        conf.check_cfg(package='libsysinfo', args='--cflags --libs')

    conf.load('autooptions')

    conf.env['LIB_PTHREAD'] = ['pthread']
    conf.env['LIB_DL'] = ['dl']
    conf.env['LIB_RT'] = ['rt']
    conf.env['LIB_M'] = ['m']
    conf.env['JACK_VERSION'] = VERSION

    conf.env['BINDIR'] = conf.env['PREFIX'] + '/bin'

    if Options.options.htmldir:
        conf.env['HTMLDIR'] = Options.options.htmldir
    else:
        # set to None here so that the doxygen code can find out the highest
        # directory to remove upon install
        conf.env['HTMLDIR'] = None

    if Options.options.libdir:
        conf.env['LIBDIR'] = Options.options.libdir
    else:
        conf.env['LIBDIR'] = conf.env['PREFIX'] + '/lib'

    if Options.options.pkgconfigdir:
        conf.env['PKGCONFDIR'] = Options.options.pkgconfigdir
    else:
        conf.env['PKGCONFDIR'] = conf.env['LIBDIR'] + '/pkgconfig'

    if Options.options.mandir:
        conf.env['MANDIR'] = Options.options.mandir
    else:
        conf.env['MANDIR'] = conf.env['PREFIX'] + '/share/man/man1'

    if conf.env['BUILD_DEBUG']:
        flags.add_c(['-g'])
        flags.add_link('-g')

    conf.env['BUILD_SIGINFO'] =  Options.options.siginfo

    conf.define('JACK_VERSION', conf.env['JACK_VERSION'])
    conf.define('SIGINFO_ENABLED', conf.env['BUILD_SIGINFO'])
    conf.write_config_header('config.h', remove=False)

    conf.recurse('dbus')

    flags.flush()

    print()
    version_msg = APPNAME + "-" + VERSION
    if os.access('version.h', os.R_OK):
        data = open('version.h').read()
        m = re.match(r'^#define GIT_VERSION "([^"]*)"$', data)
        if m != None:
            version_msg += " exported from " + m.group(1)
    elif os.access('.git', os.R_OK):
        version_msg += " git revision will be checked and eventually updated during build"
    print(version_msg)

    conf.msg('Install prefix', conf.env['PREFIX'], color='CYAN')
    conf.msg('Library directory', conf.all_envs['']['LIBDIR'], color='CYAN')
    display_feature(conf, 'Build debuggable binaries', conf.env['BUILD_DEBUG'])
    display_feature(conf, 'Build with siginfo', conf.env['BUILD_SIGINFO'])

    tool_flags = [
        ('C compiler flags',   ['CFLAGS', 'CPPFLAGS']),
        ('Linker flags',       ['LINKFLAGS', 'LDFLAGS'])
    ]
    for name, vars in tool_flags:
        flags = []
        for var in vars:
            flags += conf.all_envs[''][var]
        conf.msg(name, repr(flags), color='NORMAL')

    #conf.summarize_auto_options()

    conf.msg('D-Bus service install directory', conf.env['DBUS_SERVICES_DIR'], color='CYAN')

    if conf.env['DBUS_SERVICES_DIR'] != conf.env['DBUS_SERVICES_DIR_REAL']:
        print()
        print(Logs.colors.RED + 'WARNING: D-Bus session services directory as reported by pkg-config is')
        print(Logs.colors.RED + 'WARNING:', end=' ')
        print(Logs.colors.CYAN + conf.env['DBUS_SERVICES_DIR_REAL'])
        print(Logs.colors.RED + 'WARNING: but service file will be installed in')
        print(Logs.colors.RED + 'WARNING:', end=' ')
        print(Logs.colors.CYAN + conf.env['DBUS_SERVICES_DIR'])
        print(
            Logs.colors.RED + 'WARNING: You may need to adjust your D-Bus configuration after installing jackdbus'
            )
        print('WARNING: You can override dbus service install directory')
        print('WARNING: with --enable-pkg-config-dbus-service-dir option to this script')
        print(Logs.colors.NORMAL, end=' ')
    print()

def git_ver(self):
    bld = self.generator.bld
    header = self.outputs[0].abspath()
    if os.access('./version.h', os.R_OK):
        header = os.path.join(os.getcwd(), out, "version.h")
        shutil.copy('./version.h', header)
        data = open(header).read()
        m = re.match(r'^#define GIT_VERSION "([^"]*)"$', data)
        if m != None:
            self.ver = m.group(1)
            Logs.pprint('BLUE', "tarball from git revision " + self.ver)
        else:
            self.ver = "tarball"
        return

    if bld.srcnode.find_node('.git'):
        self.ver = bld.cmd_and_log("LANG= git rev-parse HEAD", quiet=Context.BOTH).splitlines()[0]
        if bld.cmd_and_log("LANG= git diff-index --name-only HEAD", quiet=Context.BOTH).splitlines():
            self.ver += "-dirty"

        Logs.pprint('BLUE', "git revision " + self.ver)
    else:
        self.ver = "unknown"

    fi = open(header, 'w')
    fi.write('#define GIT_VERSION "%s"\n' % self.ver)
    fi.close()

def build(bld):
    bld(rule=git_ver, target='version.h', update_outputs=True, always=True, ext_out=['.h'])

    # process subfolders from here

    if bld.env['IS_LINUX'] or bld.env['IS_FREEBSD']:
        bld.recurse('man')
    bld.recurse('dbus')

    if bld.env['BUILD_DOXYGEN_DOCS']:
        html_build_dir = bld.path.find_or_declare('html').abspath()

        bld(
            features='subst',
            source='doxyfile.in',
            target='doxyfile',
            HTML_BUILD_DIR=html_build_dir,
            SRCDIR=bld.srcnode.abspath(),
            VERSION=VERSION
        )

        # There are two reasons for logging to doxygen.log and using it as
        # target in the build rule (rather than html_build_dir):
        # (1) reduce the noise when running the build
        # (2) waf has a regular file to check for a timestamp. If the directory
        #     is used instead waf will rebuild the doxygen target (even upon
        #     install).
        def doxygen(task):
            doxyfile = task.inputs[0].abspath()
            logfile = task.outputs[0].abspath()
            cmd = '%s %s &> %s' % (task.env['DOXYGEN'][0], doxyfile, logfile)
            return task.exec_command(cmd)

        bld(
            rule=doxygen,
            source='doxyfile',
            target='doxygen.log'
        )

        # Determine where to install HTML documentation. Since share_dir is the
        # highest directory the uninstall routine should remove, there is no
        # better candidate for share_dir, but the requested HTML directory if
        # --htmldir is given.
        if bld.env['HTMLDIR']:
            html_install_dir = bld.options.destdir + bld.env['HTMLDIR']
            share_dir = html_install_dir
        else:
            share_dir = bld.options.destdir + bld.env['PREFIX'] + '/share/jack-audio-connection-kit'
            html_install_dir = share_dir + '/reference/html/'

        if bld.cmd == 'install':
            if os.path.isdir(html_install_dir):
                Logs.pprint('CYAN', 'Removing old doxygen documentation installation...')
                shutil.rmtree(html_install_dir)
                Logs.pprint('CYAN', 'Removing old doxygen documentation installation done.')
            Logs.pprint('CYAN', 'Installing doxygen documentation...')
            shutil.copytree(html_build_dir, html_install_dir)
            Logs.pprint('CYAN', 'Installing doxygen documentation done.')
        elif bld.cmd == 'uninstall':
            Logs.pprint('CYAN', 'Uninstalling doxygen documentation...')
            if os.path.isdir(share_dir):
                shutil.rmtree(share_dir)
            Logs.pprint('CYAN', 'Uninstalling doxygen documentation done.')
        elif bld.cmd == 'clean':
            if os.access(html_build_dir, os.R_OK):
                Logs.pprint('CYAN', 'Removing doxygen generated documentation...')
                shutil.rmtree(html_build_dir)
                Logs.pprint('CYAN', 'Removing doxygen generated documentation done.')
