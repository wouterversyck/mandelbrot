.PHONY: test
test: test_ptime

.PHONY: test_ptime
test_ptime: $(SRC_LS) $(TEST_LS)
	@echo '--------------------'
	@echo '-Running ptime test-'
	@echo '--------------------'
	$(CC) $(DBFLAGS) -lcmocka -Wl,--wrap=printf,--wrap=localtime,--wrap=time $(SRC_DIR)/ptime.c $(TEST_DIR)/ptime.c -o $(TEST)
	@echo ''
	./$(TEST)