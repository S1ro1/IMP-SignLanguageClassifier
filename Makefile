build_frontend:
	@echo "Building frontend..."
	@cd vue-frontend && npm run build

upload_frontend: build_frontend
	@echo "Uploading frontend..."
	@pio run --target uploadfs

build_backend:
	@echo "Building backend..."
	@pio run -e esp32cam --target upload

all: build_frontend build_backend
	@echo "Building all..."
	@pio run -e esp32cam --target monitor

run: build_backend
	@echo "Running..."
	@pio run -e esp32cam --target monitor
