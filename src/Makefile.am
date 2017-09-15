DIST_SUBDIRS = secp256k1
AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS)



BITCOIN_CONFIG_INCLUDES=-I$(builddir)/config
BITCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)
BITCOIN_INCLUDES += -I$(srcdir)/secp256k1/include

LIBBITCOIN_SERVER=libbitcoin_server.a
LIBBITCOIN_WALLET=libbitcoin_wallet.a
LIBBITCOIN_COMMON=libbitcoin_common.a
LIBBITCOIN_UTIL=libbitcoin_util.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBBITCOINQT=qt/libbitcoinqt.a

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
noinst_LIBRARIES = \
  libbitcoin_util.a \
  libbitcoin_common.a \
  libbitcoin_server.a
if ENABLE_WALLET
BITCOIN_INCLUDES += $(BDB_CPPFLAGS)
noinst_LIBRARIES += libbitcoin_wallet.a
endif

bin_PROGRAMS =
TESTS =

if BUILD_BITCOIND
  bin_PROGRAMS += hyperstaked
endif

.PHONY: FORCE
# bitcoin core #
BITCOIN_CORE_H = \
  addrman.h \
  alert.h \
  allocators.h \
  base58.h \
  bignum.h \
  bip38.h \
  bitcoinrpc.h \
  checkpoints.h \
  clientversion.h \
  coincontrol.h \
  compat.h \
  crypter.h \
  db.h \
  hashblock.h \
  init.h \
  kernel.h \
  key.h \
  keystore.h \
  main.h \
  mruset.h \
  netbase.h \
  net.h \
  pbkdf2.h \
  protocol.h \
  script.h \
  scrypt.h \
  scrypt_mine.h \
  serialize.h \
  sph_blake.h \
  sph_bmw.h \
  sph_cubehash.h \
  sph_echo.h \
  sph_groestl.h \
  sph_jh.h \
  sph_keccak.h \
  sph_luffa.h \
  sph_shavite.h \
  sph_simd.h \
  sph_skein.h \
  sph_types.h \
  sync.h \
  tinyformat.h \
  ui_interface.h \
  uint256.h \
  util.h \
  version.h \
  wallet.h \
  walletdb.h

JSON_H = \
  json/json_spirit.h \
  json/json_spirit_error_position.h \
  json/json_spirit_reader.h \
  json/json_spirit_reader_template.h \
  json/json_spirit_stream_reader.h \
  json/json_spirit_utils.h \
  json/json_spirit_value.h \
  json/json_spirit_writer.h \
  json/json_spirit_writer_template.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libbitcoin_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between bitcoind and bitcoin-qt
libbitcoin_server_a_CPPFLAGS = $(BITCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS)
libbitcoin_server_a_SOURCES = \
  addrman.cpp \
  aes_helper.c \
  alert.cpp \
  bip38.cpp \
  bitcoinrpc.cpp \
  blake.c \
  bmw.c \
  checkpoints.cpp \
  cubehash.c \
  echo.c \
  groestl.c \
  init.cpp \
  jh.c \
  keccak.c \
  kernel.cpp \
  luffa.c \
  main.cpp \
  net.cpp \
  noui.cpp \
  rpcblockchain.cpp \
  rpcmining.cpp \
  rpcnet.cpp \
  rpcrawtransaction.cpp \
  script.cpp \
  scrypt.cpp \
  shavite.c \
  simd.c \
  skein.c \
  $(JSON_H) \
  $(BITCOIN_CORE_H)

# wallet: shared between bitcoind and bitcoin-qt, but only linked
# when wallet enabled
libbitcoin_wallet_a_CPPFLAGS = $(BITCOIN_INCLUDES)
libbitcoin_wallet_a_SOURCES = \
  bip38.cpp \
  db.cpp \
  crypter.cpp \
  pbkdf2.cpp \
  rpcdump.cpp \
  rpcwallet.cpp \
  scrypt.cpp \
  wallet.cpp \
  walletdb.cpp \
  $(BITCOIN_CORE_H)

# common: shared between bitcoind, and bitcoin-qt and non-server tools
libbitcoin_common_a_CPPFLAGS = $(BITCOIN_INCLUDES)
libbitcoin_common_a_SOURCES = \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  $(BITCOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libbitcoin_util_a_CPPFLAGS = $(BITCOIN_INCLUDES)
libbitcoin_util_a_SOURCES = \
  clientversion.cpp \
  sync.cpp \
  version.cpp \
  util.cpp \
  $(BITCOIN_CORE_H)

nodist_libbitcoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# hyperstaked binary #
hyperstaked_LDADD = \
  $(LIBBITCOIN_SERVER) \
  $(LIBBITCOIN_COMMON) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_WALLET
hyperstaked_LDADD += libbitcoin_wallet.a
endif
hyperstaked_SOURCES = \
  bitcoind.cpp

#

if TARGET_WINDOWS
hyperstaked_SOURCES += bitcoind-res.rc
endif

hyperstaked_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(LIBSECP256K1)
hyperstaked_CPPFLAGS = $(BITCOIN_INCLUDES)
hyperstaked_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)


CLEANFILES = *.gcda *.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST =

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(CXXFLAGS)  -c -o $@ $<

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
