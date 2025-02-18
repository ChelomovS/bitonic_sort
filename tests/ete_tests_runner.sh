#!/bin/bash

BITONIC_EXE="./../build/bitonic_sort"

if [ ! -f "$BITONIC_EXE" ]; then
  echo "Файл $BITONIC_EXE не найден, вероятно проект не собран!"
  exit 1
fi

TEST_FILES=$(find ete_tests -name "*.in")

for TEST_FILE in $TEST_FILES; do
  echo "Обработка теста: $TEST_FILE"
  
  "$BITONIC_EXE" < "$TEST_FILE" > "${TEST_FILE%.in}.out"

  if [ $? -eq 0 ]; then
    echo "Тест завершен успешно!"
  else
    echo ">>> Тест завершен с ошибкой! <<<"
  fi
done