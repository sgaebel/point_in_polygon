clean:
	rm -rf .pytest_cache/ polygon_contains_point.cpython-310-x86_64-linux-gnu.so polygon_contains_point.egg-info/ build/
	yes | pip3 uninstall polygon_contains_point || true

install:
	pip3 install --user .

test: clean install
	pytest

wheel:
	rm dist/*
	docker build -t pointinpolygon:builder .
	docker run --rm pointinpolygon:builder &
	sleep 30
	bash scripts/build--copy-output.sh
	ls -lah dist/
