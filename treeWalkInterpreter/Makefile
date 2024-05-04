.PHONY: all
all:
	+$(MAKE) -C src
	mv src/jlox .

.PHONY: clean
clean:
	$(RM) jlox
	+$(MAKE) clean -C src
