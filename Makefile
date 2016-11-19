FLAVOR?=_dbg
BUILD_DIR=BUILD
HANDLER_NAME=handler/BUILD/ucon-handler$(FLAVOR)
RESOURCE_NAME=resource/BUILD/ucon-resource$(FLAVOR)

PRJ_DIRS=handler resource
FLAVORS=_rel _dbg

include version.mk

DIST_NAME=ucon_$(PROJECT_MAJOR).$(PROJECT_MINOR)

all: $(HANDLER_NAME) $(RESOURCE_NAME)

force:
	rm -f $(HANDLER_NAME)
	$(MAKE)

flavors:
	@for f in $(FLAVORS) ; do \
		$(MAKE) FLAVOR=$$f all || exit 1 ; \
	done
	@echo "--- flavors $(FLAVORS) ---"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(HANDLER_NAME):
	$(MAKE) -C handler FLAVOR=$(FLAVOR)

$(RESOURCE_NAME):
	$(MAKE) -C resource FLAVOR=$(FLAVOR)

clean_all: clean clean_dist
	rm -rf DIST

clean:
	@for d in $(PRJ_DIRS) ; do \
		$(MAKE) -C $$d clean || exit 1 ; \
	done

dist_dirs:
	@mkdir -p $(DIST_NAME)/L
	@mkdir -p $(DIST_NAME)/rom

dist: dist_dirs
	@for d in $(PRJ_DIRS) ; do \
		$(MAKE) -C $$d dist DIST_DIR=../$(DIST_NAME) || exit 1 ; \
	done
	@cp README.md $(DIST_NAME)/
	@echo "--- dist: $(DIST_NAME) ---"
	@ls -laR $(DIST_NAME)

dist_zip: dist
	@zip -r -9 $(DIST_NAME).zip $(DIST_NAME)

clean_dist:
	rm -rf $(DIST_NAME)
	rm -f $(DIST_NAME).zip
