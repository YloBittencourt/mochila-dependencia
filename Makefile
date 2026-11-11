CC ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -Wformat=2 -Wshadow -Wconversion -std=c99 -O2
INCLUDES = -Iinclude

SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = tests

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
APP_OBJECTS = $(filter-out $(OBJDIR)/main.o, $(OBJECTS))

TARGET = $(BINDIR)/mochila
TEST_TARGET = $(BINDIR)/test_mochila

.PHONY: all test sanitize clean help

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Alvo para compilar e rodar a suíte de testes unitários e de integração
test: $(TARGET) $(TEST_TARGET)
	@echo "===> Executando testes unitários..."
	@./$(TEST_TARGET)
	@echo "\n===> Executando teste de integração com arquivo de entrada..."
	@./$(TARGET) tests/inputs/exemplo_aula.txt

$(TEST_TARGET): $(TESTDIR)/test_mochila.c $(APP_OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

# Alvo com AddressSanitizer e UndefinedBehaviorSanitizer para checagem estrita de memória
sanitize: CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -g -fsanitize=address,undefined
sanitize: clean test
	@echo "\n[INFO] Testes finalizados com sucesso sob AddressSanitizer (zero memory leaks / buffer overflows)."

clean:
	rm -rf $(OBJDIR) $(BINDIR)

help:
	@echo "Alvos disponíveis no Makefile:"
	@echo "  make          - Compila o binário principal ($(TARGET))"
	@echo "  make test     - Compila e executa toda a suíte de testes"
	@echo "  make sanitize - Compila e testa com AddressSanitizer (detecção de leaks e falhas de memória)"
	@echo "  make clean    - Remove arquivos compilados e binários"
	@echo "  make help     - Exibe esta mensagem de ajuda"
