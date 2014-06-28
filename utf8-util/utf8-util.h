#pragma once

#include <cstdint>
#include <stdexcept>

namespace utf8
{
    typedef std::uint32_t Codepoint_t;

    bool isValidUTF8 (const char* str);

    namespace impl
    {
        int processLeading (char c, Codepoint_t& initialcp);
        bool addContinuation (char c, Codepoint_t& cp);


        template <typename container_t>
        class CodepointIteratorImpl
        {
            private:
                const container_t& m_container;

            public:
                typedef Codepoint_t value_typex;

                class iterator_impl
                {
                    private:
                        typename container_t::const_iterator m_itr;

                    public:
                        iterator_impl (const typename container_t::const_iterator& i) : m_itr (i) {}

                        value_typex operator* ()
                        {
                            Codepoint_t cp = 0;
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
}
