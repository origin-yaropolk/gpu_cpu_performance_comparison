#pragma once

#include <vector>
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
	public:
		Row(size_t columns)
			: m_columns(columns)
		{
			m_sequence.resize(m_columns, Tp{});
		}

		Tp& operator[](size_t idx) noexcept
		{
			return m_sequence[idx];
		}

		const Tp& operator[](size_t idx) const noexcept
		{
			return m_sequence[idx];
		}

		Tp& at(size_t idx)
		{
			if (idx >= m_sequence.size())
			{
				throw std::out_of_range{ "Index value is out of range" };
			}

			return m_sequence[idx];
		}

		const Tp& at(size_t idx) const
		{
			if (idx >= m_sequence.size())
			{
				throw std::out_of_range{ "Index value is out of range" };
			}

			return m_sequence[idx];
		}

		size_t size() const
		{
			return m_columns;
		}

	private:
		std::vector<Tp> m_sequence;
		size_t m_columns;
	};
	
	class Column
	{
	public:
		Column(Matrix const& matrix, size_t column)
			: m_matrix{ matrix }
			, m_column{ column }
		{
			if (m_column >= m_matrix.columns())
			{
				throw std::out_of_range{ "index column is out of range" };
			}
		}

		const Tp& operator[](size_t idx) const
		{
			return m_matrix[idx][m_column];
		}

		const Tp& at(size_t idx) const
		{
			return m_matrix.at(idx).at(m_column);
		}

	private:
		Matrix const& m_matrix;
		const size_t m_column;
	};

	Matrix(size_t rows, size_t columns)
		: m_rows{ rows }
		, m_columns{ columns }
	{
		m_internalStorage.resize(m_rows, Row(m_columns));
	}

	// navigate by elements

	Row& operator[](size_t idx) noexcept
	{
		return m_internalStorage[idx];
	}

	const Row& operator[](size_t idx) const noexcept
	{
		return m_internalStorage[idx];
	}

	Row& at(size_t idx)
	{
		return m_internalStorage.at(idx);
	}

	const Row& at(size_t idx) const
	{
		return m_internalStorage.at(idx);
	}

	Column columnAt(size_t n) const
	{
		return Column(*this, n);
	}

	size_t rows() const
	{
		return m_rows;
	}

	size_t columns() const
	{
		return m_columns;
	}

private:
	std::vector<Row> m_internalStorage;

	const size_t m_rows;
	const size_t m_columns;
};

}
