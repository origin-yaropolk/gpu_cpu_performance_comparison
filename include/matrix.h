#pragma once

#include <vector>
#include <cstddef>

namespace PerfComparison
{

	template <typename Tp>
	class Matrix
	{
	public:
		class Row
		{
		public:
			Row(Tp* begin, size_t stride)
				: m_begin{ begin }
				, m_stride{ stride }
			{}

			Tp& operator[](size_t idx)
			{
				return m_begin[idx];
			}

			Tp const& operator[](size_t idx) const
			{
				return m_begin[idx];
			}

			Tp& at(size_t idx)
			{
				if (idx >= m_stride)
				{
					throw std::out_of_range("Out of range");
				}
				return m_begin[idx];
			}

			Tp const& at(size_t idx) const
			{
				if (idx >= m_stride)
				{
					throw std::out_of_range("Out of range");
				}
				return m_begin[idx];
			}

		private:
			Tp* m_begin;
			size_t m_stride;
		};

		class Column
		{
		public:
			Column(Tp* matrix_ptr, size_t stride, size_t col_idx)
				: m_matrix_ptr{ matrix_ptr }
				, m_stride{ stride }
				, m_col_idx{ col_idx }
			{}

			Tp& operator[](size_t idx)
			{
				return m_matrix_ptr[idx * m_stride + m_col_idx];
			}

			Tp const& operator[](size_t idx) const
			{
				return m_matrix_ptr[idx * m_stride + m_col_idx];
			}

			Tp& at(size_t idx)
			{
				if (idx >= m_stride)
				{
					throw std::out_of_range("Out of range");
				}
				return m_matrix_ptr[idx];
			}

			Tp const& at(size_t idx) const
			{
				if (idx >= m_stride)
				{
					throw std::out_of_range("Out of range");
				}
				return m_matrix_ptr[idx];
			}

		private:
			Tp* m_matrix_ptr;
			size_t m_stride;
			size_t m_col_idx;
		};


		Matrix(size_t rows, size_t columns)
			: m_rows{ rows }
			, m_columns{ columns }
		{
			m_internalStorage = new Tp[m_rows * m_columns];
		}

		Row operator[](size_t idx) const
		{
			return Row{ &m_internalStorage[idx * m_columns], m_columns };
		}

		Column columnAt(size_t idx) const
		{
			if (idx >= m_columns)
			{
				throw std::out_of_range("Out of range");
			}
			return Column(m_internalStorage, m_rows, idx);
		}

		Tp* get()
		{
			return m_internalStorage;
		}

		Tp const* get() const
		{
			return m_internalStorage;
		}

		size_t columns() const
		{
			return m_columns;
		}

		size_t rows() const
		{
			return m_rows;
		}

	private:
		Tp* m_internalStorage;

		size_t m_rows;
		size_t m_columns;
	};

}