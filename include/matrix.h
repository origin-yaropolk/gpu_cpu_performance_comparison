#pragma once

#include <vector>
#include <memory>
#include <cstddef>
#include <cassert>

namespace PerfComparison
{

template <typename Tp>
class Matrix
{
public:
	class Row
	{
	private:
		friend class Matrix<Tp>;

		Row(std::shared_ptr<Tp> begin, size_t size, size_t rowIdx)
			: m_begin{ begin }
			, m_size{ size }
			, m_rowIdx{ rowIdx }
		{}

	public:

		Tp& operator[](size_t idx)
		{
			assert(!m_begin.expired());

			return m_begin.lock().get()[m_rowIdx * m_size + idx];
		}

		Tp const& operator[](size_t idx) const
		{
			assert(!m_begin.expired());

			return m_begin.lock().get()[m_rowIdx * m_size + idx];
		}

		Tp& at(size_t idx)
		{
			if (idx >= m_size)
			{
				throw std::out_of_range("Out of range");
			}

			assert(!m_begin.expired());

			return m_begin.lock().get()[m_rowIdx * m_size + idx];
		}

		Tp const& at(size_t idx) const
		{
			if (idx >= m_size)
			{
				throw std::out_of_range("Out of range");
			}

			assert(!m_begin.expired());

			return m_begin.lock().get()[m_rowIdx * m_size + idx];
		}

	private:
		std::weak_ptr<Tp> m_begin;
		size_t m_size;
		size_t m_rowIdx;
	};

	class Column
	{
	private:
		friend class Matrix<Tp>;

		Column(std::shared_ptr<Tp> matrix, size_t size, size_t columnIdx)
			: m_matrix{ matrix }
			, m_size{ size }
			, m_columnIdx{ columnIdx }
		{}

	public:

		Tp& operator[](size_t idx)
		{
			assert(!m_matrix.expired());

			return m_matrix.lock().get()[idx * m_size + m_columnIdx];
		}

		Tp const& operator[](size_t idx) const
		{
			assert(!m_matrix.expired());

			return m_matrix.lock().get()[idx * m_size + m_columnIdx];
		}

		Tp& at(size_t idx)
		{
			if (idx >= m_size)
			{
				throw std::out_of_range("Out of range");
			}

			assert(!m_matrix.expired());

			return m_matrix.lock().get()[idx];
		}

		Tp const& at(size_t idx) const
		{
			if (idx >= m_size)
			{
				throw std::out_of_range("Out of range");
			}

			assert(!m_matrix.expired());

			return m_matrix.lock().get()[idx];
		}

	private:
		std::weak_ptr<Tp> m_matrix;
		size_t m_size;
		size_t m_columnIdx;
	};

	//=================================================================

	Matrix(size_t rows, size_t columns)
		: m_internalStorage{ new Tp[rows * columns], [](Tp* pointer) { delete[] pointer; } }
		, m_rows{ rows }
		, m_columns{ columns }
	{
	}

	Row operator[](size_t idx) const &
	{
		return Row{ m_internalStorage, m_columns, idx };
	}

	Column columnAt(size_t idx) const &
	{
		if (idx >= m_columns)
		{
			throw std::out_of_range("Out of range");
		}

		return Column(m_internalStorage, m_rows, idx);
	}

	//
	// this provide base safe [] operation
	// in order to we cannot get Row with invalid internal pointer
	//
	Row operator[](size_t idx) const && = delete;
	Column columnAt(size_t idx) const && = delete;

	Tp* get()
	{
		return m_internalStorage.get();
	}

	Tp const* get() const
	{
		return m_internalStorage.get();
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
	std::shared_ptr<Tp> m_internalStorage;

	size_t m_rows;
	size_t m_columns;
};

}