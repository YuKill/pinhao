all:
	rm -f *.svg *.html *.map *.md5 *.png *.css *.js
	rm -rf search
	doxygen Doxyfile
