BUNDLENAME=widener.lv2
INSTALLDIR=~/.lv2/

.PHONY: install bundle clean uninstall reinstall test ffttest

widener.so: funcs.o
	g++ -o widener.so  -shared -fPIC -DPIC widener.cpp funcs.o `pkg-config --cflags --libs lv2-plugin fftw3 fftw3f`

clean:
	rm widener.so

bundle: widener.so
	mkdir -p 	$(BUNDLENAME)
	mv widener.so  $(BUNDLENAME)
	cp manifest.ttl widener.ttl $(BUNDLENAME)

install: bundle
	mv $(BUNDLENAME) $(INSTALLDIR)

uninstall:
	rm -rf $(INSTALLDIR)/$(BUNDLENAME)

reinstall: uninstall install

test:
	jalv http://www.mystery_plugin.co.nz/

testfunc: compiletest
	./test.o

compiletest: funcs.o
	g++ -o test.o test.cpp funcs.o `pkg-config --cflags --libs fftw3 fftw3f`

funcs.o:
	g++ -o funcs.o -c functions.cpp -fPIC `pkg-config --cflags --libs fftw3 fftw3f`