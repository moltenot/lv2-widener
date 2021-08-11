BUNDLENAME=mystery_plugin.lv2
INSTALLDIR=~/.lv2/

.PHONY: install bundle clean uninstall reinstall test ffttest

oooo.so:
	g++ -o oooo.so  -shared -fPIC -DPIC oooo.cpp `pkg-config --cflags --libs lv2-plugin fftw3 fftw3f`

clean:
	rm oooo.so

bundle: oooo.so
	mkdir -p 	$(BUNDLENAME)
	mv oooo.so  $(BUNDLENAME)
	cp manifest.ttl mystery.ttl $(BUNDLENAME)

install: bundle
	mv $(BUNDLENAME) $(INSTALLDIR)

uninstall:
	rm -rf $(INSTALLDIR)/$(BUNDLENAME)

reinstall: uninstall install

test:
	jalv http://www.mystery_plugin.co.nz/
