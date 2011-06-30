/*
 *  OpenVPN -- An application to securely tunnel IP networks
 *             over a single TCP/UDP port, with support for SSL/TLS-based
 *             session authentication and key exchange,
 *             packet encryption, packet authentication, and
 *             packet compression.
 *
 *  Copyright (C) 2002-2010 OpenVPN Technologies, Inc. <sales@openvpn.net>
 *  Copyright (C) 2010 Fox Crypto B.V. <openvpn@fox-it.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program (see the file COPYING included with this
 *  distribution); if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file Control Channel SSL library backend module
 */


#ifndef SSL_BACKEND_H_
#define SSL_BACKEND_H_

#include "buffer.h"

#ifdef USE_OPENSSL
#include "ssl_openssl.h"
#endif


/*
 *
 * Functions implemented in ssl.c for use by the backend SSL library
 *
 */

/**
 * Callback to retrieve the user's password
 *
 * @param buf		Buffer to return the password in
 * @param size		Size of the buffer
 * @param rwflag	Unused, needed for OpenSSL compatibility
 * @param u		Unused, needed for OpenSSL compatibility
 */
int pem_password_callback (char *buf, int size, int rwflag, void *u);

/*
 *
 * Functions used in ssl.c which must be implemented by the backend SSL library
 *
 */

/**
 * Perform any static initialisation necessary by the library.
 * Called on OpenVPN initialisation
 */
void tls_init_lib();

/**
 * Free any global SSL library-specific data structures.
 */
void tls_free_lib();
/**
 * Clear the underlying SSL library's error state.
 */
void tls_clear_error();

/**
 * Initialise a library-specific TLS context for a server.
 *
 * @param ctx		TLS context to initialise
 */
void tls_ctx_server_new(struct tls_root_ctx *ctx);

/**
 * Initialises a library-specific TLS context for a client.
 *
 * @param ctx		TLS context to initialise
 */
void tls_ctx_client_new(struct tls_root_ctx *ctx);

/**
 * Frees the library-specific TLSv1 context
 *
 * @param ctx		TLS context to free
 */
void tls_ctx_free(struct tls_root_ctx *ctx);

/**
 * Checks whether the given TLS context is initialised
 *
 * @param ctx		TLS context to check
 *
 * @return	true if the context is initialised, false if not.
 */
bool tls_ctx_initialised(struct tls_root_ctx *ctx);

/**
 * Set any library specific options.
 *
 * Examples include disabling session caching, the password callback to use,
 * and session verification parameters.
 *
 * @param ctx		TLS context to set options on
 * @param ssl_flags	SSL flags to set
 */
void tls_ctx_set_options (struct tls_root_ctx *ctx, unsigned int ssl_flags);

/**
 * Load Diffie Hellman Parameters, and load them into the library-specific
 * TLS context.
 *
 * @param ctx			TLS context to use
 * @param dh_file		The file name to load the parameters from, or
 * 				"[[INLINE]]" in the case of inline files.
 * @param dh_file_inline	A string containing the parameters
 */
void tls_ctx_load_dh_params(struct tls_root_ctx *ctx, const char *dh_file
#if ENABLE_INLINE_FILES
    , const char *dh_file_inline
#endif /* ENABLE_INLINE_FILES */
    );

/**
 * Load PKCS #12 file for key, cert and (optionally) CA certs, and add to
 * library-specific TLS context.
 *
 * @param ctx			TLS context to use
 * @param pkcs12_file		The file name to load the information from, or
 * 				"[[INLINE]]" in the case of inline files.
 * @param pkcs12_file_inline	A string containing the information
 *
 * @return 			1 if an error occurred, 0 if parsing was
 * 				successful.
 */
int tls_ctx_load_pkcs12(struct tls_root_ctx *ctx, const char *pkcs12_file,
#if ENABLE_INLINE_FILES
    const char *pkcs12_file_inline,
#endif /* ENABLE_INLINE_FILES */
    bool load_ca_file
    );

/*
 * Load PKCS #11 information for key and cert, and add to library-specific TLS
 * context.
 *
 * TODO: document
 */
#ifdef ENABLE_PKCS11
int tls_ctx_load_pkcs11(struct tls_root_ctx *ctx,
    bool pkcs11_id_management, const char *pkcs11_id);
#endif /* ENABLE_PKCS11 */

/**
 * Use Windows cryptoapi for key and cert, and add to library-specific TLS
 * context.
 *
 * @param ctx			TLS context to use
 * @param crypto_api_cert	String representing the certificate to load.
 */
#ifdef WIN32
void tls_ctx_load_cryptoapi(struct tls_root_ctx *ctx, const char *cryptoapi_cert);
#endif /* WIN32 */

/**
 * Load certificate file into the given TLS context. If the given certificate
 * file contains a certificate chain, load the whole chain.
 *
 * @param ctx			TLS context to use
 * @param cert_file		The file name to load the certificate from, or
 * 				"[[INLINE]]" in the case of inline files.
 * @param cert_file_inline	A string containing the certificate
 * @param x509			An optional certificate, if x509 is NULL,
 * 				do nothing, if x509 is not NULL, *x509 will be
 * 				allocated and filled with the loaded certificate.
 * 				*x509 must be NULL.
 */
void tls_ctx_load_cert_file (struct tls_root_ctx *ctx, const char *cert_file,
#if ENABLE_INLINE_FILES
    const char *cert_file_inline,
#endif
    X509 **x509
    );

/**
 * Load private key file into the given TLS context.
 *
 * @param ctx			TLS context to use
 * @param priv_key_file		The file name to load the private key from, or
 * 				"[[INLINE]]" in the case of inline files.
 * @param priv_key_file_inline	A string containing the private key
 *
 * @return 			1 if an error occurred, 0 if parsing was
 * 				successful.
 */
int tls_ctx_load_priv_file (struct tls_root_ctx *ctx, const char *priv_key_file
#if ENABLE_INLINE_FILES
    , const char *priv_key_file_inline
#endif
    );

#ifdef MANAGMENT_EXTERNAL_KEY

/**
 * Tell the management interface to load the external private key matching
 * the given certificate.
 *
 * @param ctx			TLS context to use
 * @param cert			The certificate file to load the private key for
 * 				"[[INLINE]]" in the case of inline files.
 *
 * @return 			1 if an error occurred, 0 if parsing was
 * 				successful.
 */
int tls_ctx_use_external_private_key (struct tls_root_ctx *ctx, X509 *cert);

#endif

/**
 * Show the TLS ciphers that are available for us to use in the OpenSSL
 * library.
 */
void show_available_tls_ciphers ();

/*
 * The OpenSSL library has a notion of preference in TLS ciphers.  Higher
 * preference == more secure. Return the highest preference cipher.
 */
void get_highest_preference_tls_cipher (char *buf, int size);

#endif /* SSL_BACKEND_H_ */