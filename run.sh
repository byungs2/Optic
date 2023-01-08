#!/bin/bash

for i in {1..100}
do
	./build/pr_first
	python3 ../python_testbed/numpy_test.py
done
