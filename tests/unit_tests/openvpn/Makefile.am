AUTOMAKE_OPTIONS = foreign

check_PROGRAMS=

if HAVE_LD_WRAP_SUPPORT
check_PROGRAMS += argv_testdriver buffer_testdriver
endif

check_PROGRAMS += crypto_testdriver packet_id_testdriver
if HAVE_LD_WRAP_SUPPORT
check_PROGRAMS += tls_crypt_testdriver
endif

TESTS = $(check_PROGRAMS)

openvpn_includedir = $(top_srcdir)/include
openvpn_srcdir = $(top_srcdir)/src/openvpn
compat_srcdir = $(top_srcdir)/src/compat

argv_testdriver_CFLAGS  = @TEST_CFLAGS@ -I$(openvpn_srcdir) -I$(compat_srcdir)
argv_testdriver_LDFLAGS = @TEST_LDFLAGS@ -L$(openvpn_srcdir) -Wl,--wrap=parse_line
argv_testdriver_SOURCES = test_argv.c mock_msg.c mock_msg.h \
	mock_get_random.c \
	$(openvpn_srcdir)/platform.c \
	$(openvpn_srcdir)/buffer.c \
	$(openvpn_srcdir)/argv.c

buffer_testdriver_CFLAGS  = @TEST_CFLAGS@ -I$(openvpn_srcdir) -I$(compat_srcdir)
buffer_testdriver_LDFLAGS = @TEST_LDFLAGS@ -L$(openvpn_srcdir) -Wl,--wrap=parse_line
buffer_testdriver_SOURCES = test_buffer.c mock_msg.c mock_msg.h \
	mock_get_random.c \
	$(openvpn_srcdir)/platform.c

crypto_testdriver_CFLAGS  = @TEST_CFLAGS@ \
	-I$(openvpn_includedir) -I$(compat_srcdir) -I$(openvpn_srcdir)
crypto_testdriver_LDFLAGS = @TEST_LDFLAGS@
crypto_testdriver_SOURCES = test_crypto.c mock_msg.c mock_msg.h \
	$(openvpn_srcdir)/buffer.c \
	$(openvpn_srcdir)/crypto.c \
	$(openvpn_srcdir)/crypto_mbedtls.c \
	$(openvpn_srcdir)/crypto_openssl.c \
	$(openvpn_srcdir)/otime.c \
	$(openvpn_srcdir)/packet_id.c \
	$(openvpn_srcdir)/platform.c

packet_id_testdriver_CFLAGS  = @TEST_CFLAGS@ \
	-I$(openvpn_includedir) -I$(compat_srcdir) -I$(openvpn_srcdir)
packet_id_testdriver_LDFLAGS = @TEST_LDFLAGS@
packet_id_testdriver_SOURCES = test_packet_id.c mock_msg.c mock_msg.h \
	mock_get_random.c \
	$(openvpn_srcdir)/buffer.c \
	$(openvpn_srcdir)/otime.c \
	$(openvpn_srcdir)/packet_id.c \
	$(openvpn_srcdir)/platform.c

tls_crypt_testdriver_CFLAGS  = @TEST_CFLAGS@ \
	-I$(openvpn_includedir) -I$(compat_srcdir) -I$(openvpn_srcdir)
tls_crypt_testdriver_LDFLAGS = @TEST_LDFLAGS@ \
	-Wl,--wrap=buffer_read_from_file \
	-Wl,--wrap=buffer_write_file \
	-Wl,--wrap=parse_line \
	-Wl,--wrap=rand_bytes
tls_crypt_testdriver_SOURCES = test_tls_crypt.c mock_msg.c mock_msg.h \
	$(openvpn_srcdir)/argv.c \
	$(openvpn_srcdir)/base64.c \
	$(openvpn_srcdir)/buffer.c \
	$(openvpn_srcdir)/crypto.c \
	$(openvpn_srcdir)/crypto_mbedtls.c \
	$(openvpn_srcdir)/crypto_openssl.c \
	$(openvpn_srcdir)/env_set.c \
	$(openvpn_srcdir)/otime.c \
	$(openvpn_srcdir)/packet_id.c \
	$(openvpn_srcdir)/platform.c \
	$(openvpn_srcdir)/run_command.c
