import platform
import os
import sys

version="2.0.4"
abi_major = 3
abi_minor = 0

cflags = '-g -Wall -DDEBUG'

slmsource = [
    'src/slm/ids2ngram/ids2ngram.cpp',
    'src/slm/ids2ngram/idngram_merge.cpp',
    'src/slm/mmseg/mmseg.cpp',
    'src/slm/tslminfo/tslminfo.cpp',
    'src/slm/tslmpack/arpa_slm.cpp',
    'src/slm/tslmpack/arpa_conv.cpp',
    'src/slm/tslmpack/slmpack.cpp',
    'src/slm/slm.cpp',
    'src/slm/slminfo/slminfo.cpp',
    'src/slm/sim_sen.cpp',
    'src/slm/sim_slm.cpp',
    'src/slm/getwordfreq/getwordfreq.cpp',
    'src/slm/slmseg/slmseg.cpp',
    'src/slm/thread/slmthread.cpp',
    'src/slm/thread/test_vc.cpp',
    'src/slm/thread/ValueCompress.cpp',
    'src/slm/slmbuild/slmbuild.cpp',
    'src/slm/slmprune/slmprune.cpp',
    'src/slm/sim_slmbuilder.cpp',
    'src/slm/tslmendian/slm_endian.cpp',
    'src/slm/tslmendian/writer.cpp',
    'src/slm/tslmendian/slm_file.cpp',
    'src/slm/sim_dict.cpp',
    'src/portability.cpp',
    'src/common/lexicon/trie_writer.cpp',
    'src/common/lexicon/gentrie.cpp',
    'src/common/lexicon/trie_factory.cpp',
    'src/common/lexicon/maker_factory.cpp',
    'src/common/lexicon/trie_maker.cpp',
    'src/common/lexicon/trie.cpp',
    'src/common/lexicon/CUnitData.cpp',
    'src/common/lexicon/CUnigramSorter.cpp',
    'src/common/lexicon/tree/PNodeSet.cpp',
    'src/pinyin/lexicon/pytrie_maker.cpp',
    'src/pinyin/input/pinyin_data.cpp',
    'src/xh/lexicon/oc/CXhocTrieMaker.cpp',
    'src/xh/lexicon/oc/path.cpp',
    'src/xh/lexicon/xh/CXhTrieMaker.cpp',
    'src/xh/lexicon/DictConv.cpp',
    'src/xh/input/xh_data.cpp',
    'src/xh/input/xhoc_data.cpp',
    ]

imesource = [
    'src/portability.cpp',
    'src/slm/slm.cpp',
    'src/common/lexicon/trie.cpp',
    'src/common/lexicon/CUnitData.cpp',
    'src/common/input/segment/string_seg.cpp',
    'src/common/input/segment/seperator_seg.cpp',
    'src/common/input/segment/tail_seg.cpp',
    'src/ime-core/lattice/lattice.cpp',
    'src/ime-core/lattice/lexicon_states.cpp',
    'src/ime-core/lattice/lattice_states.cpp',
    'src/ime-core/lattice/lattice_manager.cpp',
    'src/ime-core/helper/CFullCharManager.cpp',
    'src/ime-core/helper/CInputTrieSource.cpp',
    'src/pinyin/input/pinyin_data.cpp',
    'src/pinyin/input/pinyin_seg.cpp',
    'src/pinyin/input/shuangpin_data.cpp',
    'src/pinyin/input/shuangpin_seg.cpp',
    'src/pinyin/input/hunpin_seg.cpp',
    'src/pinyin/input/segment/TPySyllableSegment.cpp',
    'src/pinyin/lattice/TPyLexiconState.cpp',
    'src/pinyin/lattice/CPyLatticeManager.cpp',
    'src/xh/input/xh_seg.cpp',
    'src/xh/input/xh_data.cpp',
    'src/xh/input/xhoc_data.cpp',
    'src/xh/input/segment/TXhSyllableSegment.cpp',
    'src/xh/input/segment/pathnode.cpp',
    'src/xh/input/segment/path.cpp',
    'src/xh/lattice/TXhLexiconState.cpp',
    'src/xh/lattice/CXhLatticeManager.cpp',
    'src/ime-core/TCandiRank.cpp',
    'src/ime-core/imi_context.cpp',
    'src/ime-core/imi_data.cpp',
    'src/ime-core/imi_uiobjects.cpp',
    'src/ime-core/ic_history.cpp',
    'src/ime-core/userdict.cpp',
    'src/ime-core/imi_funcobjs.cpp',
    'src/ime-core/view/imi_view.cpp',
    'src/ime-core/view/imi_view_classic.cpp',
    'src/ime-core/view/imi_view_xh.cpp',
    'src/ime-core/imi_winHandler.cpp',
    'src/ime-core/options/imi_option_event.cpp',
    'src/ime-core/options/lang_policy_cn.cpp',
    'src/ime-core/options/lang_policy_cn_xh.cpp',
    'src/ime-core/options/scheme_policy_hp.cpp',
    'src/ime-core/options/scheme_policy_qp.cpp',
    'src/ime-core/options/scheme_policy_sp.cpp',
    'src/ime-core/options/scheme_policy_xh.cpp',
    'src/ime-core/options/a.cpp',
    ]

headers = [
    'src/slm/ids2ngram/idngram.h',
    'src/slm/ids2ngram/idngram_merge.h',
    'src/slm/slm.h',
    'src/slm/tslmpack/arpa_slm.h',
    'src/slm/tslmpack/common.h',
    'src/slm/tslmpack/arpa_conv.h',
    'src/slm/sim_dict.h',
    'src/slm/sim_sen.h',
    'src/slm/sim_slm.h',
    'src/slm/thread/ValueCompress.h',
    'src/slm/sim_fmerge.h',
    'src/slm/sim_slmbuilder.h',
    'src/slm/tslmendian/slm_file.h',
    'src/slm/tslmendian/writer.h',
    'src/common/lexicon/trie_maker.h',
    'src/common/lexicon/trie_writer.h',
    'src/common/lexicon/trie.h',
    'src/common/lexicon/thread/TrieThreadModel.h',
    'src/common/lexicon/thread/TThreadNode.h',
    'src/common/lexicon/thread/TTransUnit.h',
    'src/common/lexicon/thread/TUnit.h',
    'src/common/lexicon/thread/TUnitInfo.h',
    'src/common/lexicon/thread/TWordIdInfo.h',
    'src/common/lexicon/tree/TrieTreeModel.h',
    'src/common/lexicon/tree/CTreeNode.h',
    'src/common/lexicon/tree/PNodeSet.h',
    'src/common/lexicon/tree/TTreeWordId.h',
    'src/common/lexicon/tree/TWordInfo.h',
    'src/ime-core/view/imi_view.h',
    'src/ime-core/view/imi_view_classic.h',
    'src/ime-core/view/imi_view_xh.h',
    'src/ime-core/imi_uiobjects.h',
    'src/ime-core/lattice/lattice_states.h',
    'src/ime-core/lattice/lattice.h',
    'src/ime-core/lattice/lexicon_states.h',
    'src/ime-core/lattice/lattice_manager.h',
    'src/ime-core/helper/CFullCharManager.h',
    'src/ime-core/helper/CInputTrieSource.h',
    'src/ime-core/ic_history.h',
    'src/ime-core/imi_funcobjs.h',
    'src/ime-core/imi_context.h',
    'src/ime-core/candidate.h',
    'src/ime-core/TCandiPair.h',
    'src/ime-core/TCandiRank.h',
    'src/ime-core/imi_winHandler.h',
    'src/ime-core/imi_glibHandler.h',
    'src/ime-core/userdict.h',
    'src/ime-core/options/imi_option_event.h',
    'src/ime-core/options/imi_option_keys.h',
    'src/ime-core/options/lang_policy_cn.h',
    'src/ime-core/options/lang_policy_cn_xh.h',
    'src/ime-core/options/profile_class.h',
    'src/ime-core/options/profile_interface.h',
    'src/ime-core/options/scheme_policy_hp.h',
    'src/ime-core/options/scheme_policy_qp.h',
    'src/ime-core/options/scheme_policy_sp.h',
    'src/ime-core/options/scheme_policy_xh.h',
    'src/ime-core/options/style_policy_classic.h',
    'src/ime-core/options/style_policy_xh.h',
    'src/ime-core/options/session_factory.h',
    'src/ime-core/imi_data.h',
    'src/ime-core/utils.h',
    'src/ime-core/imi_keys.h',
    'src/ime-core/imi_defines.h',
    'src/portability.h',
    'src/common/input/segmentor.h',
    'src/common/input/segment/segment.h',
    'src/common/input/segment/invalid_seg.h',
    'src/common/input/segment/segment_types.h',
    'src/common/input/segment/seperator_seg.h',
    'src/common/input/segment/string_seg.h',
    'src/common/input/segment/syllable_seg.h',
    'src/common/input/segment/tail_seg.h',
    'src/pinyin/input/shuangpin_seg.h',
    'src/pinyin/input/datrie.h',
    'src/pinyin/input/quanpin_trie.h',
    'src/pinyin/input/pinyin_info.h',
    'src/pinyin/input/pinyin_seg.h',
    'src/pinyin/input/pinyin_data.h',
    'src/pinyin/input/syllable.h',
    'src/pinyin/input/shuangpin_data.h',
    'src/pinyin/input/hunpin_seg.h',
    'src/pinyin/input/datrie_impl.h',
    'src/pinyin/input/segment/TPySyllableSegment.h',
    'src/xh/input/xh_data.h',
    'src/xh/input/xhoc_data.h',
    'src/xh/input/xh_seg.h',
    'src/xh/input/segment/pathnode.h',
    'src/xh/input/segment/path.h',
    'src/xh/input/segment/checkpoint.h',
    'src/xh/input/segment/TXhSyllableSegment.h',
    'src/sunpinyin.h',
    ]

# source of plugin module, it's off by default
# and this module depends on Python
imesource_plugin = [
    'src/ime-core/imi_plugin.cpp',
    ]

headers_plugin = [
    'src/ime-core/imi_plugin.h',
    ]

bins = [
    'src/dict/mmseg',
    'src/dict/slmseg',
    'src/dict/ids2ngram',
    'src/dict/idngram_merge',
    'src/dict/slmbuild',
    'src/dict/slmprune',
    'src/dict/slminfo',
    'src/dict/slmthread',
    'src/dict/tslmendian',
    'src/dict/tslminfo',
    'src/dict/tslmpack',
    'src/dict/genpyt',
    'src/dict/dconv',
    'src/dict/getwordfreq',
    'src/dict/sunpinyin-dictgen',
    ]

man1s = [
    'man/mmseg.1',
    'man/slmseg.1',
    'man/ids2ngram.1',
    'man/idngram_merge.1',
    'man/slmbuild.1',
    'man/slmprune.1',
    'man/slminfo.1',
    'man/slmthread.1',
    'man/tslmendian.1',
    'man/tslminfo.1',
    'man/tslmpack.1',
    'man/genpyt.1',
    'man/getwordfreq.1',
    ]

docs = [
    'doc/README',
    'doc/SLM-train.mk',
    'doc/SLM-inst.mk',
    ]

# options
AddOption('--prefix', dest='prefix', metavar='DIR',
          help='installation prefix')

AddOption('--libdir', dest='libdir', metavar='DIR',
          help='installation libdir')

AddOption('--datadir', dest='datadir', metavar='DIR',
          help='installation data dir')

AddOption('--rpath', dest='rpath', metavar='DIR',
          help='encode rpath in the executables')

AddOption('--enable-plugins', dest='enable_plugins', action='store_true',
          default=False, help='enable plugin mechanism at libsunpinyin layer')

AddOption('--disable-plugins', dest='enable_plugins', action='store_false',
          default=False, help='disable plugin mechanism at libsunpinyin layer')

# save the options
opts = Variables('configure.conf')
opts.Add('PREFIX', default='/usr/local')
opts.Add('LIBDIR', default='/usr/local/lib')
opts.Add('DATADIR', default='/usr/local/share')
opts.Add('ENABLE_PLUGINS', default=False)

#
#==============================environment==============================
#
#
def allinc():
    if 0:
        inc=[]
        for root, dirs, files in os.walk('src'):
            inc.append(root)
        return inc
    return ['src']

def GetOS():
    return platform.uname()[0]

def CreateEnvironment():
    make = 'make'
    wget = 'wget'
    tar = 'tar'
    if GetOS() == 'Darwin':
        wget = 'curl -O'
    elif GetOS() == 'FreeBSD':
        make = 'gmake'
        wget = 'fetch'
    elif GetOS() == 'SunOS':
        make = 'gmake'
        tar = 'gtar'
    libln_builder = Builder(action='cd ${TARGET.dir} && ln -s ${SOURCE.name} ${TARGET.name}')
    env = Environment(ENV = os.environ, CFLAGS = cflags, CXXFLAGS = cflags,
                      MAKE = make, WGET = wget, TAR = tar,
                      CPPPATH = ['.'] + allinc(),
                      tools = ['default', 'textfile'])
    env.Append(BUILDERS = {'InstallAsSymlink': libln_builder})
    env['ENDIANNESS'] = "be" if sys.byteorder == "big" else "le"
    return env

def PassVariables(envvar, env):
    for (x, y) in envvar:
        if x in os.environ:
            print 'Warning: you\'ve set %s in the environmental variable!' % x
            env[y] = os.environ[x]

env = CreateEnvironment()
opts.Update(env)

if GetOption('prefix') is not None:
    env['PREFIX'] = GetOption('prefix')
    env['LIBDIR'] = os.path.join(env['PREFIX'], 'lib')
    env['DATADIR'] = os.path.join(env['PREFIX'], 'share')

if GetOption('libdir') is not None:
    env['LIBDIR'] = GetOption('libdir')

if GetOption('datadir') is not None:
    env['DATADIR'] = GetOption('datadir')

env['ENABLE_PLUGINS'] = GetOption('enable_plugins')

opts.Save('configure.conf', env)

bindir = os.path.join(env['PREFIX'], 'bin')
mandir = os.path.join(env['PREFIX'], 'share/man')
man1dir = os.path.join(mandir, 'man1')
docdir = os.path.join(env['PREFIX'], 'share/doc/sunpinyin')
headersdir = os.path.join(env['PREFIX'], 'include/sunpinyin-2.0')
datadir = os.path.join(env['DATADIR'], 'sunpinyin')
libdir = env['LIBDIR']

# pass through environmental variables
envvar = [('CC', 'CC'),
          ('CXX', 'CXX'),
          ('CFLAGS', 'CFLAGS'),
          ('CXXFLAGS', 'CXXFLAGS'),
          ('LDFLAGS', 'LINKFLAGS'),
          ('TAR', 'TAR'),
          ('MAKE', 'MAKE'),
          ('WGET', 'WGET')]
PassVariables(envvar, env)

# append the source and headers
if env['ENABLE_PLUGINS']:
    imesource += imesource_plugin
    headers += headers_plugin

# merge some of critical compile flags
env.MergeFlags(['-pipe -DHAVE_CONFIG_H',
                '-DSUNPINYIN_DATA_DIR=\\\'\\"%s\\"\\\'' % datadir])

if GetOption('rpath') is not None and GetOS() != 'Darwin':
    env.MergeFlags('-Wl,-R -Wl,%s' % GetOption('rpath'))

#
#==============================configure================================
#
def CheckPKGConfig(context, version='0.12.0'):
    context.Message('Checking for pkg-config... ')
    ret = context.TryAction('pkg-config --atleast-pkgconfig-version=%s' % version)[0]
    context.Result(ret)
    return ret

def CheckPKG(context, name):
    context.Message('Checking for %s... ' % name)
    ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
    context.Result(ret)
    return ret

def CheckPython(context):
    context.Message('Checking for Python library...')
    ret = context.TryAction('python-config --prefix')[0]
    context.Result(ret)
    if ret:
        context.env.MergeFlags(['!python-config --includes',
                                '!python-config --libs'])
    return ret

def AppendEndianCheck(conf):
    conf.config_h_text += r'''

#if defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
# define WORDS_BIGENDIAN 1

#elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
# undef WORDS_BIGENDIAN

#elif defined(__sparc) || defined(__sparc__) \
  || defined(_POWER)   || defined(__powerpc__) \
  || defined(__ppc__)  || defined(__hpux) || defined(__hppa) \
  || defined(_MIPSEB)  || defined(_POWER) \
  || defined(__s390__) || (defined(__sh__) && defined(__BIG_ENDIAN__))
# define WORDS_BIGENDIAN 1

#elif defined(__i386__) || defined(__i386) \
  || defined(__ia64)    || defined(__ia64__) \
  || defined(_M_IX86)   || defined(_M_IA64) \
  || defined(_M_ALPHA)  || defined(__amd64) \
  || defined(__amd64__) || defined(_M_AMD64) \
  || defined(__x86_64)  || defined(__x86_64__) \
  || defined(_M_X64)    || defined(__bfin__) \
  || defined(__alpha__) || defined(__ARMEL__) \
  || defined(_MIPSEL)   || (defined(__sh__) && defined(__LITTLE_ENDIAN__))
# undef WORDS_BIGENDIAN

#else
# error can not detect the endianness!
#endif
'''

conf = env.Configure(clean=False, help=False, config_h='config.h',
                     custom_tests={'CheckPKGConfig' : CheckPKGConfig,
                                   'CheckPKG' : CheckPKG,
                                   'CheckPython': CheckPython})

def DoConfigure():
    if GetOS() == 'Darwin':
        if not conf.CheckLibWithHeader('sqlite3', 'sqlite3.h', 'C'):
            Exit(1)
        if not conf.CheckLibWithHeader('iconv', 'iconv.h', 'C'):
            Exit(1)
    else:
        if not conf.CheckPKGConfig():
            Exit(1)
        if not conf.CheckPKG('sqlite3'):
            Exit(1)

    if conf.env['ENABLE_PLUGINS']:
        if not conf.CheckPython():
            Exit(1)
        conf.Define('ENABLE_PLUGINS')

    conf.Define('ENABLE_NLS', 1)
    conf.Define('GETTEXT_PACKAGE', '"sunpinyin2"')
    conf.CheckCHeader('assert.h')
    conf.CheckFunc('bind_textdomain_codeset')
    conf.CheckFunc('dcgettext')
    conf.CheckCHeader('dlfcn.h')
    conf.CheckFunc('exp2')
    conf.CheckCHeader('fcntl.h')
    conf.CheckCHeader('getopt.h')
    conf.CheckFunc('getopt_long')
    conf.CheckFunc('getpagesize')
    conf.CheckFunc('get_opt')
    conf.CheckCHeader('iconv.h')
    conf.CheckCHeader('inttypes.h')
    conf.CheckCHeader('locale.h')
    conf.CheckCHeader('libintl.h')
    conf.CheckCHeader('limits.h')
    conf.CheckCHeader('locale.h')
    conf.CheckFunc('log2')
    conf.CheckCHeader('memory.h')
    conf.CheckFunc('memset')
    conf.CheckFunc('mmap')
    conf.CheckFunc('munmap')
    conf.CheckFunc('setlocale')
    conf.CheckFunc('strndup')
    conf.CheckCHeader('sys/mman.h')
    conf.CheckCHeader('sys/param.h')
    conf.CheckCHeader('sys/stat.h')
    conf.CheckCHeader('sys/types.h')
    conf.CheckCHeader('unistd.h')
    conf.CheckCHeader('wchar.h')

    # add essential package requirements
    conf.Define('PACKAGE', '"sunpinyin"')
    conf.Define('PACKAGE_NAME', '"sunpinyin"')
    conf.Define('PACKAGE_STRING', '"sunpinyin 2.0"')
    conf.Define('PACKAGE_TARNAME', '"sunpinyin"')
    conf.Define('PACKAGE_VERSION', '"2.0"')
    conf.Define('VRESION', '"2.0"')

    # append endianness checking defines
    AppendEndianCheck(conf)
    env = conf.Finish()

    if GetOS() != 'Darwin':
        env.ParseConfig('pkg-config sqlite3 --libs --cflags')

if not GetOption('clean') and not GetOption('help'):
    DoConfigure()

#
#==============================compile==============================
#
env.Object(slmsource)
env.Command('src/pinyin/input/quanpin_trie.h', 'python/quanpin_trie_gen.py',
            'cd ${SOURCE.dir} && ./quanpin_trie_gen.py > ../src/pinyin/input/quanpin_trie.h')
env.Command('src/pinyin/input/pinyin_info.h', 'python/pinyin_info_gen.py',
            'cd ${SOURCE.dir} && ./pinyin_info_gen.py > ../src/pinyin/input/pinyin_info.h')

SConscript(['src/SConscript', 'man/SConscript', 'doc/SConscript'], exports='env')

env.Substfile('sunpinyin-2.0.pc.in', SUBST_DICT={
        '@PREFIX@': env['PREFIX'],
        '@LIBDIR@': env['LIBDIR'],
        '@VERSION@': version,
        '@CFLAGS@': reduce(lambda a, b: a + ' ' + b,
                           map(lambda x: '-I$${includedir}' + x[3:],
                               allinc())),
        })

libname_default = '%ssunpinyin%s' % (env.subst('${SHLIBPREFIX}'),
                                     env.subst('${SHLIBSUFFIX}'))
libname_link = libname_default
libname_soname = '%s.%d' % (libname_link, abi_major)
libname = '%s.%d' % (libname_soname, abi_minor)
lib = None

if GetOS() != 'Darwin':
    lib = env.SharedLibrary(libname, SHLIBSUFFIX='', source=imesource,
                            parse_flags='-Wl,-soname=%s' % libname_soname)
else:
    # TODO: add install_name on Darwin?
    lib = env.SharedLibrary('sunpinyin', source=imesource)

def DoInstall():
    lib_target = None
    if GetOS() == 'Darwin':
        lib_target = env.Install(libdir, lib)
    else:
        lib_target_bin = env.Install(libdir, lib)
        # where does it goes
        install_path = os.path.dirname(str(lib_target_bin[0]))
        lib_target = [
            lib_target_bin,
            env.InstallAsSymlink(os.path.join(install_path, libname_soname),
                                 lib_target_bin),
            env.InstallAsSymlink(os.path.join(install_path, libname_link),
                                 lib_target_bin),
            ]

    lib_pkgconfig_target = env.Install(os.path.join(libdir, 'pkgconfig'),
                                       ['sunpinyin-2.0.pc'])
    bin_target = env.Install(bindir, bins)
    man1_target = env.Install(man1dir, man1s)
    doc_target = env.Install(docdir, docs)

    header_targets = []
    for header in headers:
        header_targets.append(env.InstallAs(headersdir + header[3:], header))
    env.Alias('install-headers', header_targets)
    env.Alias('install-bin', bin_target)
    env.Alias('install-man1', man1_target)
    env.Alias('install-doc', doc_target)
    env.Alias('install-lib', lib_target + [lib_pkgconfig_target])
    Mkdir(datadir)

DoInstall()
env.Alias('install', [
    'install-bin', 'install-man1', 'install-doc', 'install-headers', 'install-lib'
])

# -*- indent-tabs-mode: nil -*- vim:et:ts=4
