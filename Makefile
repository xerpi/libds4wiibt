all:
	$(MAKE) -C libds4wiibt	install
	$(MAKE) -C sample

run:
	$(MAKE) -C libds4wiibt	install
	$(MAKE) -C sample		run

clean:
	$(MAKE) -C libds4wiibt	clean
	$(MAKE) -C sample		clean
