.PHONY: audit preflight clean

audit:
	bash ./tools/run_audit.sh

preflight:
	bash ./preflight/build.sh

clean:
	$(RM) -r build
