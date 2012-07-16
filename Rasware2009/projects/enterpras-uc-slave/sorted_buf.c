#include "sorted_buf.h"

void sb_init(sorted_buf_t *sb)
{
	int i;
	for(i = 0; i<N; ++i)
	{
		sb->v[i] = 0;
		sb->p[i] = sb->v+i;
	}
}

void sb_insert_at(sorted_buf_t *sb, const short v, const int n)
{
	sb->v[n] = v;
}

int sb_find(const sorted_buf_t *sb, const int n)
{
	// store pointer for easy access
	const short * const p = sb->v+n;
	int i;
	for(i=0; i<N; ++i)
	{
		if(p == sb->p[i]) break;
	}
	return i;
}

void sb_sort(sorted_buf_t *sb, int n)
{
	const short * p;
	// n is an index into sb->v, which is unordered
	// need to find the corresponding pointer in sb->p
	n = sb_find(sb, n);
	// now n is index into sb->p	  
	// pointer to original value for easy access
	p = sb->p[n];

	while(1)
	{
		if((n > 0) && (*p < (*sb->p[n-1])))
		{	// the value should be swapped left
			sb->p[n] = sb->p[n-1];
			--n;
		} else if ((n < N-1) && (*p > (*sb->p[n+1])))
		{	// the value should be swapped right
			sb->p[n] = sb->p[n+1];
			++n;
		} else {
			// the value shouldn't be swapped either way!
			// store it in its final location
			sb->p[n] = p;
			break;
		}
	}
	return;
}

short sb_get_median(const sorted_buf_t *sb)
{
	return *sb->p[N/2];
}

short sb_update_and_get_median(sorted_buf_t *sb, const short v, const int n)
{
	sb_insert_at(sb, v, n);
	sb_sort(sb, n);
	return sb_get_median(sb);
}
