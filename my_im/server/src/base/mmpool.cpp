#include "mmpool.h"
struct mp_pool_s *mp_create_pool(size_t size) {

	struct mp_pool_s *p;
	int ret = posix_memalign((void **)&p, MP_ALIGNMENT, size + sizeof(struct mp_pool_s) + sizeof(struct mp_node_s));
	if (ret) {
		return nullptr;
	}
	
	p->max = (size < MP_MAX_ALLOC_FROM_POOL) ? size : MP_MAX_ALLOC_FROM_POOL;
	p->current = p->head;
	p->large = nullptr;
    p->head->next = nullptr;
	p->head->last = (unsigned char *)p + sizeof(struct mp_pool_s) + sizeof(struct mp_node_s);
	p->head->end = p->head->last + size;

	p->head->failed = 0;

	return p;

}

void mp_destory_pool(struct mp_pool_s *pool) {

	struct mp_node_s *h, *n;
	struct mp_large_s *l;

	for (l = pool->large; l; l = l->next) {
		if (l->alloc) {
			free(l->alloc);
		}
	}

	h = pool->head->next;  //为什么从第二个开始释放，因为第一个连着池结构，在后边直接释放池结构即可。

	while (h) {
		n = h->next;
		free(h);
		h = n;
	}

	free(pool);

}

void mp_reset_pool(struct mp_pool_s *pool) {

	struct mp_node_s *h;
	struct mp_large_s *l;

	for (l = pool->large; l; l = l->next) {
		if (l->alloc) {
			free(l->alloc);
		}
	}

	pool->large = nullptr;

	for (h = pool->head; h; h = h->next) {
		h->last = (unsigned char *)h + sizeof(struct mp_node_s);
		//h->last = (unsigned char *)h + sizeof(struct mp_node_s) + sizeof(struct mp_pool_s);我感觉
	}

}

static void *mp_alloc_block(struct mp_pool_s *pool, size_t size) {

	unsigned char *m;
	struct mp_node_s *h = pool->head;
	size_t psize = (size_t)(h->end - (unsigned char *)h);
	
	int ret = posix_memalign((void **)&m, MP_ALIGNMENT, psize);
	if (ret) return nullptr;

	struct mp_node_s *p, *new_node, *current;
	new_node = (struct mp_node_s*)m;

	new_node->end = m + psize;
	new_node->next = nullptr;
	new_node->failed = 0;

	m += sizeof(struct mp_node_s);
	m = (unsigned char*)mp_align_ptr(m, MP_ALIGNMENT);
	new_node->last = m + size;

	current = pool->current;

	for (p = current; p->next; p = p->next) {
		if (p->failed++ > 4) {
			current = p->next;
		}
	}
	p->next = new_node;

	pool->current = current ? current : new_node;

	return m;

}

static void *mp_alloc_large(struct mp_pool_s *pool, size_t size) {

	void *p = malloc(size);
	if (p == nullptr) return nullptr;

	size_t n = 0;
	struct mp_large_s *large;
	for (large = pool->large; large; large = large->next) {
		if (large->alloc == nullptr) {
			large->alloc = p;
			return p;
		}
		if (n ++ > 3) break;
	}

	large = (struct mp_large_s*)mp_alloc(pool, sizeof(struct mp_large_s)); //把大块的结构体放在小块的内存中
	if (large == nullptr) {
		free(p);
		return nullptr;
	}

	large->alloc = p;
	large->next = pool->large;
	pool->large = large;

	return p;
}

void *mp_memalign(struct mp_pool_s *pool, size_t size, size_t alignment) {

	void *p;
	
	int ret = posix_memalign(&p, alignment, size);
	if (ret) {
		return nullptr;
	}

	struct mp_large_s *large = (struct mp_large_s*)mp_alloc(pool, sizeof(struct mp_large_s));
	if (large == nullptr) {
		free(p);
		return nullptr;
	}

	large->alloc = p;
	large->next = pool->large;
	pool->large = large;

	return p;
}




void *mp_alloc(struct mp_pool_s *pool, size_t size) {

	unsigned char *m;
	struct mp_node_s *p;

	if (size <= pool->max) {

		p = pool->current;

		do {
			
			m = (unsigned char*)mp_align_ptr(p->last, MP_ALIGNMENT); //32
			if ((size_t)(p->end - m) >= size) {
				p->last = m + size;
				return m;
			}
			p = p->next;
		} while (p);

		return mp_alloc_block(pool, size);
	}

	return mp_alloc_large(pool, size);
	
}


void *mp_nalloc(struct mp_pool_s *pool, size_t size) {

	unsigned char *m;
	struct mp_node_s *p;

	if (size <= pool->max) {
		p = pool->current;

		do {
			m = p->last;
			if ((size_t)(p->end - m) >= size) {
				p->last = m+size;
				return m;
			}
			p = p->next;
		} while (p);

		return mp_alloc_block(pool, size);
	}

	return mp_alloc_large(pool, size);
	
}

void *mp_calloc(struct mp_pool_s *pool, size_t size) {

	void *p = mp_alloc(pool, size);
	if (p) {
		memset(p, 0, size);
	}

	return p;
	
}

void mp_free(struct mp_pool_s *pool, void *p) {

	struct mp_large_s *l;
	for (l = pool->large; l; l = l->next) {
		if (p == l->alloc) {
			free(l->alloc);
			l->alloc = nullptr;

			return ;
		}
	}
	
}