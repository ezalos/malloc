#ifndef PROTOTYPES_MALLOC_H
# define PROTOTYPES_MALLOC_H

t_alloc_header	*alloc_access_next(t_alloc_header *alloc);
t_alloc_header	*alloc_access_th(t_zone *zone, size_t zone_size,
				size_t umpteenth);
void			alloc_header_init(t_alloc_header *header, size_t size,
				uint8_t available, uint8_t last);
int8_t			alloc_split(t_alloc_header *alloc,
				size_t first_alloc_header_size);
void			free_alloc(t_alloc_header *alloc_header);
t_alloc_header	*get_alloc_header(void* alloc, t_zone *zone);
void			*get_spot(size_t size_to_find);
int				main(int ac, char **av);
int8_t			malloc_exit(void);
// int8_t			malloc_init(void);
int8_t			mem_index_add(t_alloc_header *availble_alloc);
uint8_t			mem_index_del(t_alloc_header *unavailble_alloc);
t_alloc_header	*mem_index_get(size_t size);
void 			mem_put_color(t_alloc_header *alloc, int8_t alloc_nb);
void			our_free(void *alloc);
void			*our_malloc(size_t size);
void			print_alloc_header(t_alloc_header *alloc, size_t *total_octet);
void			print_alloc_memory(t_alloc_header *alloc, size_t *total_octet,
				size_t alloc_nb);
void			print_malloc_mem(void);
void			print_zone(t_zone *zone, size_t zone_size);
t_malloc		**static_mem(void);
t_malloc		*static_mem_test(void);
void			test_read(void *mem, size_t size);
void			test_write(void *mem, size_t size);
int8_t			zone_create(t_zone **zone, size_t zone_size);
void			zone_header_init(t_zone_header *header);
int8_t			zone_liberate_all(t_zone *zone, size_t zone_size);

size_t			malloc_good_size(size_t size);

#endif
