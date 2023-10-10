ls: my_ls.c
	gcc -o my_ls my_ls.c -I.

clean:
	rm -f *.o

fclean: clean
	rm -f my_ls

re: fclean all