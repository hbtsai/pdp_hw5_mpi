#include <cstdio>
#include <cstdlib>
#include <cstring>

int main() {
	// ========================================
	// Read input graph
	// ========================================
	int n;
	scanf("%d%*c", &n);
	bool *searched = new bool[n]();
	int *degree = new int[n]();
	int **neighbors = new int*[n];
	int *id_queue = new int[n];
	int id_queue_size;
	char *line_buffer = new char[n * 11];
	for (int i = 0; i < n; i++) {
		fgets(line_buffer, n * 11, stdin);
		char *str_search_ptr = strchr(line_buffer, '>');
		char *next_delim;
		if (str_search_ptr) {
			id_queue_size = 0;
			str_search_ptr++;
			while (true) {
				id_queue[id_queue_size++] = atoi(str_search_ptr);
				next_delim = strchr(str_search_ptr, ',');
				if (next_delim) {
					str_search_ptr = next_delim + 1;
				} else {
					break;
				}
			}
			degree[i] = id_queue_size;	
			neighbors[i] = new int[id_queue_size];
			memcpy(neighbors[i], id_queue, sizeof(int) * id_queue_size);
		}
		/*
		char delim;
		while (true) {
			delim = getchar();
			if (delim == '-') {
				getchar();
				id_queue_size = 0;	
				do {
					scanf("%d", &id_queue[id_queue_size++]);
				} while (getchar() == ',');
				degree[i] = id_queue_size;	
				neighbors[i] = new int[id_queue_size];
				memcpy(neighbors[i], id_queue, sizeof(int) * id_queue_size);
				break;
			} else if (delim == '\n' || delim == EOF) {
				break;
			}
		}
		*/

	}

	// ========================================
	// Count connected component using DFS
	// ========================================
	int ans = 0;
	id_queue_size = 0;
	for (int i = 0; i < n; i++) {
		if (!searched[i]) {
			ans++;
			searched[i] = true;
			id_queue[id_queue_size++] = i;
		}
		while (id_queue_size) {
			int parent = id_queue[--id_queue_size];
			for (int j = 0; j < degree[parent]; j++) {
				int child = neighbors[parent][j];
				if (!searched[child]) {
					searched[child] = true;
					id_queue[id_queue_size++] = child;
				}
			}
		}
	}

	// ========================================
	// Output answer
	// ========================================
	printf("%d\n", ans);
	return 0;
}

