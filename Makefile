#!/usr/bin/make

version = $(error version is not set)

.DEFAULT_GOAL:= build
.PHONY: install build clean

build: mod_authn_indieauth.la

clean:
	rm src/*.l* src/*.slo || true
	rm -R src/.libs || true

install:
	apxs -i -a src/mod_authn_indieauth.la

mod_authn_indieauth.la: src/mod_authn_indieauth.c
	apxs -c src/mod_authn_indieauth.c

dependencies:
	sudo apt install build-essential fakeroot devscripts apxs apache2-dev dupload

new-version:
	dch -v $(version)

package:
	debuild --no-tgz-check -us -uc -Zxz

upload:
	dupload --to debian-mentors
