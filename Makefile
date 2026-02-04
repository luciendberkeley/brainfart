run:
	cc -o main main.c
	@echo "Input: "; \
	read INPUT; \
	./main.exe "test.bf" $$INPUT
