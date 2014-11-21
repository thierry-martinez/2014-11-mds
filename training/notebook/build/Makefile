IMAGES=compilation_chain compilation_graph

all: $(IMAGES:=.png) $(IMAGES:=.pdf)

clean:
	rm -f $(IMAGES:=.png) $(IMAGES:=.pdf)

%.pdf: %.tex
	pdflatex $<

%.png: %.pdf
	convert -density 100 $< $@
