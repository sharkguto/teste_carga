# teste_carga
teste de carga



#run java
gradle bootRun

#pytest test_*.py --cov=. --cov-report=

pytest --cov=. tests/ --cov-fail-under 80 --cov-report xml --cov-report term-missing
