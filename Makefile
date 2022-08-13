
all: _DO_RUN test speed

test: _DO_RUN compile vector_test stack_test pair_test tree_test set_test map_test ctree_test

speed: _DO_RUN compile vector_speed pair_speed set_speed ctree_speed

compile: _DO_RUN vector_compile stack_compile pair_compile tree_compile set_compile map_compile ctree_compile

clean: _DO_RUN
	$(MAKE) -C vector fclean
	$(MAKE) -C stack fclean
	$(MAKE) -C pair fclean
	$(MAKE) -C rb_tree fclean
	$(MAKE) -C set fclean
	$(MAKE) -C map fclean
	$(MAKE) -C rb_tree/ctree fclean

# ------------------------------------------------------------
vector: _DO_RUN vector_compile vector_test vector_speed

stack: _DO_RUN stack_compile stack_test

pair: _DO_RUN pair_compile pair_speed

tree: _DO_RUN tree_compile tree_test

set: _DO_RUN set_compile set_test set_speed

map: _DO_RUN map_compile map_test

ctree: _DO_RUN ctree_compile ctree_test ctree_speed

# --------------------------------------------------------------
vector_compile: _DO_RUN
	$(MAKE) -C vector

stack_compile: _DO_RUN
	$(MAKE) -C stack

pair_compile: _DO_RUN
	$(MAKE) -C pair

tree_compile: _DO_RUN
	$(MAKE) -C rb_tree

set_compile: _DO_RUN
	$(MAKE) -C set

map_compile: _DO_RUN
	$(MAKE) -C map

ctree_compile: _DO_RUN
	$(MAKE) -C rb_tree/ctree

# ----------------------------------------------------------------
vector_test: _DO_RUN
	./vector/test

stack_test: _DO_RUN
	./stack/test

pair_test: _DO_RUN
	./pair/test

tree_test: _DO_RUN
	./rb_tree/test

set_test: _DO_RUN
	./set/test

map_test: _DO_RUN
	./map/test

ctree_test: _DO_RUN
	./rb_tree/ctree/test

# -------------------------------------------------------------
vector_speed: _DO_RUN
	./vector/speed

pair_speed: _DO_RUN
	./pair/speed

set_speed: _DO_RUN
	./set/speed

ctree_speed: _DO_RUN
	./rb_tree/ctree/speed

_DO_RUN:

