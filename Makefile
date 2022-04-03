help:           ## Help screen
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

build-image: ## Build docker CI image locally 
	DOCKER_BUILDKIT=1 docker-compose -f docker-compose.yml build 

build-app: ## Builds STM32 App
	DOCKER_BUILDKIT=1 docker-compose -f docker-compose.yml up

push: ## Push image 
	DOCKER_BUILDKIT=1 docker-compose -f docker-compose.yml push

pull: ## Pull image 
	DOCKER_BUILDKIT=1 docker-compose -f docker-compose.yml pull
