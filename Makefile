#!/usr/bin/make

version = $(error version is not set)

.DEFAULT_GOAL:= build
.PHONY: install build clean

build: indieauth_client.la mod_authn_indieauth.la

clean:
	rm src/*.l* src/*.slo || true
	rm -R src/.libs || true

install: src/mod_authn_indieauth.la
	apxs -i -a src/mod_authn_indieauth.la

mod_authn_indieauth.la: src/mod_authn_indieauth.c src/mod_authn_indieauth.h
	apxs -c src/mod_authn_indieauth.c

indieauth_client.la: src/indieauth_client.c src/indieauth_client.h
	apxs -c src/indieauth_client.c

dependencies:
	sudo apt install build-essential fakeroot devscripts apxs apache2-dev dupload

new-version:
	dch -v $(version)

package:
	debuild --no-tgz-check -us -uc -Zxz

upload:
	dupload --to debian-mentors
