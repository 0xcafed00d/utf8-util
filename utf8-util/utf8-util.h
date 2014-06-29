#pragma once

#include <cstdint>
#include <stdexcept>

namespace utf8
{
    bool isValidUTF8 (const char* str);

    namespace impl
    {
        const bool throwOnErr = true;

        int processLeading (char c, char32_t& initialcp, bool throwOnErr = false);
        bool addContinuation (char c, char32_t& cp, bool throwOnErr = false);
        int countContinuations (char c, bool throwOnErr = false);
        bool isContinuation (char c);

        template <typename container_t>
        class CodepointIteratorImpl
        {
            private:
                const container_t& m_container;

            public:
                class iterator_impl
                {
                    private:
                        typename container_t::const_iterator m_itr;
                        typename container_t::const_iterator m_endItr;

                        void verifyItrInc (typename container_t::const_iterator& i, const char* errmsg)
                        {
                            if (i == m_endItr)
                                throw std::runtime_error(errmsg);

                            ++i;
                        }

                    public:
                        iterator_impl (const typename container_t::const_iterator& i, const typename container_t::const_iterator& end) : m_itr (i), m_endItr (end) {}

                        char32_t operator* ()
                        {
                            auto itr = m_itr;
                            char32_t cp = 0;
                            int ncont = impl::processLeading(*itr, cp, throwOnErr);
                            while (ncont)
                            {
                                verifyItrInc(itr, "incomplete codepoint at end of input");
                                ncont--;
                                impl::addContinuation(*itr, cp, throwOnErr);
                            }
                            return cp;
                        }

                        iterator_impl& operator++()
                        {
                            int ncont = impl::countContinuations(*m_itr, throwOnErr);
                            verifyItrInc(m_itr, "attempt to move past end of input");
                            while (ncont)
                            {
                                verifyItrInc(m_itr, "incomplete codepoint at end of input");
                                ncont--;
                            }
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
                    return iterator_impl(m_container.cbegin(), m_container.cend());
                }

                iterator_impl end () const
                {
                    return iterator_impl(m_container.cend(), m_container.cend());
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
        int cc = 0;
        while (begin != end)
        {
            cc = impl::countContinuations(*begin);
            if (cc < 0)
                return false;

            ++begin;
            while (cc)
            {
                if ((begin != end) && impl::isContinuation(*begin))
                {
                    ++begin;
                    cc--;
                }
                else
                    return false;
            }
        }
        return cc == 0;
    }

    template <typename container_t>
    bool isValidUTF8 (const container_t& c)
    {
        return isValidUTF8(c.begin(), c.end());
    }
}
