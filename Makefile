help: ## make [option] 
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

all: build-image build-app doxy-doc ## Build local docker image, build app from it, doxy-doc

build-image: ## Build docker CI image locally 
	DOCKER_BUILDKIT=1 docker-compose -f docker-compose.yml build 

build-app: ## Builds STM32 App 
	DOCKER_BUILDKIT=1 docker-compose -f docker-compose.yml up

pull: ## Pull latest image from ghcr
	DOCKER_BUILDKIT=1 docker-compose -f docker-compose.yml pull

doxy-doc: ## Generate Doxygen documentation
	docker run --rm -it -v "$(PWD):/stm32-app" $(shell docker build -q -f Doxygen/Dockerfile .) /bin/sh -c "doxygen Doxygen/config/Doxyfile"
