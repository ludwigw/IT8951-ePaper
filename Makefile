.PHONY: apidocs docs-clean

apidocs:
	doxygen Doxyfile

docs-clean:
	rm -rf docs/api 