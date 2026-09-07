.PHONY: audit preflight clean

audit:
	./tools/run_audit.sh

preflight:
	./preflight/build.sh

clean:
	$(RM) -r build
