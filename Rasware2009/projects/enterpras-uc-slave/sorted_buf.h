#ifndef SORTED_BUF_H
#define SORTED_BUF_H

#define N 50

typedef struct sorted_buf
{
	short v[N];
	const short *p[N];

} sorted_buf_t;

/// Initializes a sorted circular buffer.
/// \param sb a sorted_buf_t to initialize
/// \return the initialized sorted_buf_t, for convenience
void sb_init(sorted_buf_t *sb);

/// Insert a value at a specified position in the circular
/// buffer.
/// NOTE: This does not modify the ordering denoted by sb->p
///       and does not maintain the sorting!
///
/// \param sb a sorted_buf_t
/// \param v the new value
/// \param n the index into the circular buffer where to insert v
void sb_insert_at(sorted_buf_t *sb, const short v, const int n);

/// Sorts the pointers in a sorted circular buffer such that
/// *sb->p[0] <= *sb->p[1] <= ... <= *sb->p[N-1]
/// assuming that only one value is out of place
/// \param sb an almost sorted sorted_buf_t
/// \param n index to the value which isn't sorted
void sb_sort(sorted_buf_t *sb, int n);

/// Get the median value in the circular buffer.
/// NOTE: Assumes that the cricular buffer is sorted!
///
/// \param sb a sorted_buf_t
/// \return the median value among sb->v
short sb_get_median(const sorted_buf_t *sb);

/// Updates a circular sorted buffer by inserting a new value
/// at a specified location, re-sorts the buffer, then locates
/// the median.
/// \param sb a pre-sorted sorted_buf_t
/// \param v the new value
/// \param the index into the circular buffer of the old value to
///        replace
/// \return the median value in the (updated) sorted circular buffer
short sb_update_and_get_median(sorted_buf_t *sb, short v, int n);

#endif
