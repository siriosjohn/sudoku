GCC =-gcc -Wall -ansi -pedantic

sudoku:	sudoku.o grid.o
	$(GCC) -o sudoku $^

%.o: %.c %.h
	$(GCC) $< -c  -o $@

clean:
	@echo "Removing files..."
	@rm *.o
	@rm sudoku
	@echo "Files removed succesfully"