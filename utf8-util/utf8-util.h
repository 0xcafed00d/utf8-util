#pragma once

#include <cstdint>
#include <stdexcept>

namespace utf8
{
    bool isValidUTF8 (const char* str);

    namespace impl
    {
        int processLeading (char c, char32_t& initialcp);
        bool addContinuation (char c, char32_t& cp);
        int countContinuations (char c);
        bool isContinuation (char c);

        template <typename container_t>
        class CodepointIteratorImpl
        {
            private:
                const container_t& m_container;

            public:
                typedef char32_t value_typex;

                class iterator_impl
                {
                    private:
                        typename container_t::const_iterator m_itr;

                    public:
                        iterator_impl (const typename container_t::const_iterator& i) : m_itr (i) {}

                        value_typex operator* ()
                        {
                            char32_t cp = 0;
                            int ncont = impl::processLeading(*m_itr, cp);
                            while (ncont)
                            {
                                ++m_itr;
                                ncont--;
                                impl::addContinuation(*m_itr, cp);
                            }
                            return cp;
                        }

                        iterator_impl& operator++()
                        {
                            ++m_itr;
                            return *this;
                        }

                        iterator_impl& operator--()
                        {
                            --m_itr;
                            return *this;
                        }

                        bool operator== (const iterator_impl& that)
                        {
                            return m_itr == that.m_itr;
                        }

                        bool operator!= (const iterator_impl& that)
                        {
                            return m_itr != that.m_itr;
                        }
                };

                CodepointIteratorImpl (const container_t& c) : m_container(c)
                {
                }

                iterator_impl begin () const
                {
                    return iterator_impl(m_container.cbegin());
                }

                iterator_impl end () const
                {
                    return iterator_impl(m_container.cend());
                }
        };
    }

    template <typename container_t>
    inline impl::CodepointIteratorImpl<container_t> CodepointIterator (const container_t& c)
    {
        return impl::CodepointIteratorImpl<container_t>(c);
    }

    template <typename iterator_t>
    bool isValidUTF8 (iterator_t begin, iterator_t end)
    {
        while (begin != end)
        {
            int cc = impl::countContinuations(*begin);
            if (cc < 0)
                return false;

            ++begin;
            while (cc)
            {
                if (impl::isContinuation(*begin))
                {
                    ++begin;
                    cc--;
                }
                else
                    return false;
            }
        }
        return true;
    }

    template <typename container_t>
    bool isValidUTF8 (const container_t& c)
    {
        return isValidUTF8(c.begin(), c.end());
    }
}
