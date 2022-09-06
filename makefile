qSim:
	gcc src/qSim.c -lm -o bin/qSim
	@echo "\nRunning three test cases:\n\nTESTCASE 1:"
	bin/qSim 1000 4 100 5
	@echo "\n\nTESTCASE 2:"
	bin/qSim 100 100 60 1
	@echo "\n\nTESTCASE 3:"
	bin/qSim 10 100 60 5
	@echo "\n\ngnuPlot"
	make gnuPlot
	
gnuPlot:
	(cd output;make;cd ..)
clean:
	rm -r bin/qSim output/*txt output/*pdf
	
