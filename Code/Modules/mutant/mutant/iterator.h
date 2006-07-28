#ifndef MUTANT_ITERATOR_H_
#define MUTANT_ITERATOR_H_

namespace mutant
{
	////////////////////////////////////////////////
	template<typename Container>
	struct iterator
	{
		typedef Container ex_cont;
		typedef typename ex_cont::iterator		ex_iter;
		typedef typename ex_cont::value_type	value_type;
		typedef iterator<ex_cont>				this_type;

		iterator( ex_iter const& b, ex_iter const& e )
		:	mBegin( b ), mEnd( e ) { }

		iterator( this_type const& r )
		:	mBegin( r.mBegin ), mEnd( r.mEnd ) { }

		value_type const* operator->() const { return &*mBegin; }
		value_type /*_ const& */ operator*() const { return *mBegin; }
		value_type* operator->() { return &*mBegin; }
//		value_type& operator*() { return *mBegin; }
		bool good() const { return mBegin != mEnd; }
		operator bool() const { return good(); }

		this_type const& operator++() { ++mBegin; return *this; }
		this_type operator++(int) { this_type rv(*this); ++mBegin; return rv; }

	private:
		ex_iter		mBegin;
		ex_iter		mEnd;
	};

	////////////////////////////////////////////////
	template<typename Container>
	struct const_iterator
	{
	public:
		typedef Container ex_cont;
		typedef typename ex_cont::const_iterator	ex_iter;
		typedef typename ex_cont::value_type		value_type;
		typedef mutant::const_iterator<ex_cont>		this_type;
		
		const_iterator( ex_iter const& b, ex_iter const& e )
			:	mBegin( b ), mEnd( e ) { }
		
		const_iterator( this_type const& r )
			:	mBegin( r.mBegin ), mEnd( r.mEnd ) { }
		
		value_type const* operator->() const { return &*mBegin; }
		value_type const& operator*() const { return *mBegin; }
		bool good() const { return mBegin != mEnd; }
		operator bool() const { return good(); }
		
		this_type const& operator++() { ++mBegin; return *this; }
		this_type operator++(int) { this_type rv(*this); ++mBegin; return rv; }
		
	private:
		ex_iter		mBegin;
		ex_iter		mEnd;
	};
}

#endif // MUTANT_ITERATOR_H_
