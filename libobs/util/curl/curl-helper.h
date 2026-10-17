/*
 * Copyright (c) 2023 Lain Bailey <lain@obsproject.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#pragma once

#include <curl/curl.h>

#if defined(_WIN32) && LIBCURL_VERSION_NUM >= 0x072c00

#ifdef CURLSSLOPT_REVOKE_BEST_EFFORT
#define CURL_OBS_REVOKE_SETTING CURLSSLOPT_REVOKE_BEST_EFFORT
#else
#define CURL_OBS_REVOKE_SETTING CURLSSLOPT_NO_REVOKE
#endif

#define curl_obs_set_revoke_setting(handle) curl_easy_setopt(handle, CURLOPT_SSL_OPTIONS, CURL_OBS_REVOKE_SETTING)

#else

#define curl_obs_set_revoke_setting(handle)

#endif

#ifdef ENABLE_RELOCATABLE

#include <stdlib.h>

/* A relocatable build (the AppImage) carries a libcurl from the distribution
 * it was built on, which looks for that distribution's CA bundle only. Its
 * launcher names the host's bundle in SSL_CERT_FILE: verify peers with that. */
static inline void curl_obs_set_ca_bundle(CURL *handle)
{
	const char *bundle = getenv("SSL_CERT_FILE");

	if (bundle && *bundle) {
		curl_easy_setopt(handle, CURLOPT_CAINFO, bundle);
	}
}

#else

#define curl_obs_set_ca_bundle(handle)

#endif
