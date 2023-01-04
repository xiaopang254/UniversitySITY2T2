# Python Environment

To prevent overloading libraries into the main computer system, I use python venv to create virtual environment for me to install necessary python files for specific folders only.

This way, I won't need to deal with installation clashing with other libraries in my computer system.
Installation:
```
pip install virtualenv
OR
pip3 install virtualenv
```

Create and Run Virtual Environment:
```
python3 -m venv env
source env/bin/activate
```

To check your list of pip installations:
```
pip list
```

Your pip should be empty if you have activate it properly for the first time!

```
pip freeze > requirements.txt
```

# Pip Libraries
See requirements.txt

# Docker
```
docker compose up
```