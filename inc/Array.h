#include <memory>
#include <array>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <iostream>

#include "type.h"

template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v) {
    s.put('[');
    char comma[3] = {'\0', ' ', '\0'};
    for (const auto& e : v) {
        s << comma << e;
        comma[0] = ',';
    }
    return s << ']';
}

u8 const
    READ(0),
    WRITE(1);

template<typename T>
struct Host;

template<typename T1, typename T2>
void memCopy(T1 & dst, T2 const & src, size_t nbElement, size_t offsetDst, size_t offsetSrc);

template<typename T>
void memFill(Host<T> & dst, T value, size_t nbElement, size_t offsetDst);




template<typename A, size_t NbDim>
size_t prod(A const (& array)[NbDim], size_t offset = 0)
{
    size_t res(1);
    for(size_t i(offset); i < NbDim; ++i)
        res *= array[i];
    return res;
}

static size_t prod(std::vector<size_t> const & vect, size_t offset = 0)
{
	size_t res(1);
	for(size_t i(offset); i < vect.size(); ++i)
		res *= vect[i];
	return res;
}
static i32 azeazeazeazeazeaze = prod(std::vector<size_t>(),0);

template<typename T, typename Board>
struct Container
{
    Container():
        nbElement_(0),
        board_()
    {azeazeazeazeazeaze;}

	template<typename... OtherArgs>
    Container(size_t nbElement, OtherArgs... args):
        nbElement_(nbElement),
        board_(nbElement_, args...)
    {}

    ~Container(){}

    T* ptr() const
    {
        return board_.ptr_;
    }

    size_t nbElement() const
    {
        return nbElement_;
    }

    size_t size() const
    {
        return nbElement_ * sizeof(T);
    }

    size_t nbElement_;
    Board board_;
};

struct Space
{
    Space():
        dim_(), start_(), stop_()
    {}

    Space(size_t const nbElement):
        dim_(1, nbElement), start_(1, 0), stop_(1, nbElement)
    {}

    template<typename A, size_t NbDim>
    Space(A const (& nbElements)[NbDim]):
        dim_(NbDim), start_(NbDim), stop_(NbDim)
    {
    	for(size_t i(0); i < NbDim; ++i)
        {
            dim_[i] = nbElements[i];
            stop_[i] = nbElements[i];
        }
    }

    Space(
        std::vector<size_t> const & dim,
        std::vector<size_t> const & start,
        std::vector<size_t> const & stop
    ):dim_(dim), start_(start),  stop_(stop)
    {}


    Space(
        std::vector<size_t> && dim,
        std::vector<size_t> && start,
        std::vector<size_t> && stop
    ):dim_(std::move(dim)), start_(std::move(start)),  stop_(std::move(stop))
    {}

    Space(Space & bs):
        dim_(bs.dim_), start_(bs.start_), stop_(bs.stop_)
    {}

    Space(Space && bs):
        dim_(std::move(bs.dim_)), start_(std::move(bs.start_)), stop_(std::move(bs.stop_))
    {}

    Space & operator=(Space const & bs)
    {
        dim_ = bs.dim_;
        start_ = bs.start_;
        stop_ = bs.stop_;
        return *this;
    }

    ~Space(){}

    Space operator[](size_t pos) const
    {
        size_t changeDim(firstActiveDim());
        std::vector<size_t>
            newStart(start_),
            newStop(stop_);
        newStart[changeDim] += + pos;
        newStop[changeDim] = newStart[changeDim];
        return Space(std::vector<size_t>(dim_), std::move(newStart), std::move(newStop));
    }

    Space at(
        std::vector<size_t> const & start,
        std::vector<size_t> const & stop
    ) const {
        //Note how many dimension we have
        size_t const nbActiveD(nbActiveDim());

        //Test if space have enought dimension for args
        if(start.size() > nbActiveD && stop.size() > nbActiveD)
            std::cout << "Error !\n";

        //Create new dimensions
        std::vector<size_t>
            newStart(start_),
            newStop(start_);

        //Create 2 iterators, one for space dimensions, other for args
        size_t
            dimsIndex(firstActiveDim()),
            argsIndex(0);

        //For each argsIndex dimensions
        for(; argsIndex < nbActiveD; ++dimsIndex)
        {
            //Only compute if current dim is active
            if(isActive(dimsIndex))
            {
                //Add new start offset
                newStart[dimsIndex] += start[argsIndex];
                //Add new end offset
                newStop[dimsIndex] += stop[argsIndex];
                //Throw error if new stop > current stop
                if(newStop[dimsIndex] > stop_[dimsIndex])
                    std::cout << "Error stop too large ! : " << newStop[dimsIndex] << " < " << stop_[dimsIndex] << std::endl;

                argsIndex++;
            }
        }
        //If/for remaining active dimensions, use current dimensions
        for(;dimsIndex < nbActiveD; ++dimsIndex)
            newStop[dimsIndex] = stop_[dimsIndex];

        return Space(std::vector<size_t>(dim_), std::move(newStart), std::move(newStop));
    }

    Space at(
        size_t start,
        size_t stop
    ) const {
        //Note how many dimension we have
        size_t
            dim(firstActiveDim());

        //Create new dimensions
        std::vector<size_t>
            newStart(start_),
            newStop(stop_);

        newStart[dim] += start;
        newStop[dim] = newStart[dim] + (stop - start);

        return Space(std::vector<size_t>(dim_), std::move(newStart), std::move(newStop));
    }

    bool operator==(Space const & oSpace) const
    {
        std::vector<size_t>
            shape1(shape()),
            shape2(oSpace.shape());
        if(shape1.size() != shape2.size())
            return false;
        for(size_t i(0); i < shape1.size(); ++i)
            if(shape1[i] != shape2[i])
                return false;
        return true;
    }

    bool operator!=(Space const & oSpace) const
    {
        return !(*this == oSpace);
    }

    //Return the dims of the data
    std::vector<size_t> const & dim() const
    {
        return dim_;
    }

    //Return the current dim of data
    std::vector<size_t> shape() const
    {
        std::vector<size_t> shape(nbActiveDim());

        size_t j(0);
        for(size_t i(0); i < dim_.size(); ++i)
            if(isActive(i))
                shape[j++] = lengthAt(i);

        return shape;
    }

    //The dimension size @ dim
    size_t dimAt(size_t dim) const
    {
        if(dim < dim_.size())
            return dim_[dim];
        else
            return 0;
    }

    //The current dim @ dim
    size_t lengthAt(size_t dim) const
    {
        return stop_[dim] - start_[dim];
    }

    //Return the current dim @ dim active dimension
    size_t shapeAt(size_t dim) const
    {
        size_t i(0), j(0);
        for(; i < dim_.size(); ++i)
            if(isActive(i))
                if(j++ == dim)
                    return lengthAt(i);

        return 0;
    }

    size_t currentLength() const
    {
        return lengthAt(firstActiveDim());
    }

    size_t nbDim() const
    {
        return dim_.size();
    }

    size_t lastCompleteDim() const
    {
        size_t d(dim_.size());
        while(d > 0 && isDimComplete(--d));
        return d;
    }

    size_t nbActiveDim() const
    {
        size_t activeDim(0);
        for(size_t i(0); i < dim_.size(); ++i)
            if(isActive(i))
                activeDim++;
        return activeDim;
    }

    size_t nbActiveDim(size_t const maxDim) const
    {
        size_t activeDim(0);
        for(size_t i(0); i < maxDim; ++i)
            if(isActive(i))
                activeDim++;
        return activeDim;
    }

    bool isActive(size_t dim) const
    {
        return stop_[dim] > start_[dim];
    }

    bool isDimComplete(size_t dim) const
    {
        return start_[dim] == 0 && stop_[dim] == dim_[dim];
    }

    size_t offset() const
    {
        size_t offset(start_[0]);
        for(size_t d(1); d < dim_.size(); ++d)
            offset = offset * dim_[d] + start_[d];
        return offset;
    }

    size_t offsetAt(size_t dim) const
    {
        size_t offset(start_[0]);
        for(size_t d(1); d < dim; ++d)
            offset = offset * dim_[d] + start_[d];
        return offset;
    }

    size_t dimOffset() const
    {
        size_t dim(dim_.size() - 1), offset(1);
        while(dim > 0 && isDimComplete(dim))
            offset *= dim_[dim--];
        return offset * start_[dim];
    }

    size_t nbRangedElement() const
    {
        size_t dim(dim_.size() - 1), range(lengthAt(dim));
        while(dim > 0 && isDimComplete(dim--))
            range *= lengthAt(dim);
        return range;
    }

    size_t nbNotRangedDim() const
    {
        size_t dim(dim_.size() - 1), notRangedDim(0);

        while(dim > 0 && isDimComplete(dim--));

        while(dim > 0 && isActive(dim--))
        	++notRangedDim;

        return notRangedDim;
    }



    size_t firstActiveDim() const
    {
        for(size_t i(0); i < dim_.size(); ++i)
            if(isActive(i))
                return i;
        return dim_.size();
    }

    size_t lastActiveDim() const
    {
        for(size_t i(dim_.size() - 1); i >= 0; --i)
            if(isActive(i))
                return i;
        return 0;
    }

    size_t nextActiveDim(size_t currentDim) const
    {
        for(++currentDim ;currentDim < dim_.size(); ++currentDim)
            if (isActive(currentDim))
                return currentDim;
        return 0;
    }

    size_t prevActiveDim(size_t currentDim) const
    {
        for(--currentDim ;currentDim >= 0; --currentDim)
            if (isActive(currentDim))
                return currentDim;
        return dim_.size();
    }

    bool hasActiveDim() const
    {
        return firstActiveDim() != dim_.size();
    }

    bool isLastDimActive() const
    {
        return isActive(dim_.size() - 1);
    }

    std::vector<size_t> dim_;
    std::vector<size_t> start_;
    std::vector<size_t> stop_;
};

template<typename T, typename Board> class BaseArray;
//template<typename T, typename Board> class Array;

template<typename T, typename Board>
class BaseArray
{
public:
    //Void constructor
    BaseArray():
        container_(nullptr),
        space_()
    {}

    //1 dimensions constructor with init array and optionnal board specific args
    template<typename... OtherArgs>
    BaseArray(size_t nbElement, T * array, OtherArgs... args):
        container_(std::make_shared<Container<T, Board>>(nbElement, args...)),
        space_(nbElement)
    {
        setMem(array);
    }

    //N dimensions constructor with init array and optionnal board specific args
    template<typename A, size_t NbDim, typename... OtherArgs>
    BaseArray(A const (&&nbElements)[NbDim], T * array, OtherArgs... args):
        container_(std::make_shared<Container<T, Board>>(prod(nbElements), args...)),
        space_(nbElements)
    {
        setMem(array);
    }

    //1 dimensions constructor with init value and optionnal board specific args
    template<typename... OtherArgs>
    BaseArray(size_t nbElement, T value, OtherArgs... args):
        container_(std::make_shared<Container<T, Board>>(nbElement, args...)),
        space_(nbElement)
    {
        setValue(value);
    }

    //N dimensions constructor with init value and optionnal board specific args
    template<typename A, size_t NbDim, typename... OtherArgs>
    BaseArray(A const (&&nbElements)[NbDim], T value, OtherArgs... args):
        container_(std::make_shared<Container<T, Board>>(prod(nbElements), args...)),
        space_(nbElements)
    {
        setValue(value);
    }

    //1 dimensions constructor with optionnal board specific args
	template<typename... OtherArgs>
    BaseArray(size_t nbElement, OtherArgs... args):
        container_(std::make_shared<Container<T, Board>>(nbElement, args...)),
        space_(nbElement)
    {}

    //N dimensions constructor with optionnal board specific args
    template<typename A, size_t NbDim, typename... OtherArgs>
    BaseArray(A const (&&nbElements)[NbDim], OtherArgs... args):
        container_(std::make_shared<Container<T, Board>>(prod(nbElements), args...)),
        space_(nbElements)
    {}

    BaseArray(
        std::shared_ptr<Container<T, Board>> container,
        Space space
    ):
        container_(std::move(container)),
        space_(std::move(space))
    {}

    BaseArray(
    	BaseArray<T, Board> & ob
    ):
        container_(ob.container_),
        space_(ob.space_)
    {}

    BaseArray(
    	BaseArray<T, Board> && ob
    ):
        container_(std::move(ob.container_)),
        space_(std::move(ob.space_))
    {}

    ~BaseArray(){}

    //Copy
    BaseArray<T, Board> & operator=(BaseArray<T, Board> & oArray)
    {
    	if(container_)
    	{
    		accessArray<WRITE, Board>(oArray);
    	}
    	else
    	{
    		//Case with no container_
    		container_ = oArray.container_;
    		space_ = oArray.space_;
    	}
    	return *this;
    }

    BaseArray<T, Board> & operator=(BaseArray<T, Board> && oArray)
    {
    	if(container_)
    	{
    		accessArray<WRITE, Board>(oArray);
    	}
    	else
    	{
    		//Case with no container_
    		container_ = oArray.container_;
    		space_ = oArray.space_;
    	}
    	return *this;
    }

    template<typename OtherBoard>
    BaseArray<T, Board> & operator=(BaseArray<T, OtherBoard> & oArray)
    {
    	accessArray<WRITE, OtherBoard>(oArray);
    	return *this;
    }

    template<typename OtherBoard>
    BaseArray<T, Board> & operator=(BaseArray<T, OtherBoard> && oArray)
    {
    	accessArray<WRITE, OtherBoard>(oArray);
    	return *this;
    }

    template<typename A, size_t NbDim>
    BaseArray<T, Board> at(
    	A const (&&start)[NbDim],
    	A const (&&stop)[NbDim]
    ) const {
    	//Convert array to vector
    	std::vector<size_t>
    		vectStart(space_.start_),
    		vectStop(space_.stop_);

		for(size_t i(0); i < NbDim; ++i)
		{
			vectStart[i] = start[i];
			vectStop[i] = stop[i];
		}

        return BaseArray<T, Board>(container_, std::move(space_.at(std::move(vectStart), std::move(vectStop))));
    }

    BaseArray<T, Board> at(size_t start, size_t stop) const
    {
        return BaseArray<T, Board>(container_, space_.at(start, stop));
    }

    BaseArray<T, Board> operator[](size_t pos) const
    {
        return BaseArray<T, Board>(container_, std::move(space_[pos]));
    }

    BaseArray<T, Board> base() const
	{
    	std::vector<size_t>
    		baseStart(space_.dim_.size(), 0),
    		baseStop(space_.dim_);

    	return BaseArray<T, Board>(container_, std::move(space_.at(std::move(baseStart), std::move(baseStop))));
	}

    template<typename... OtherArgs>
    void setMem(T * array, OtherArgs... args)
    {
        accessArray<WRITE, OtherArgs...>(array, args...);
    }

    template<typename... OtherArgs>
    void copyTo(T * array, OtherArgs... args)
    {
        accessArray<READ, OtherArgs...>(array, args...);
    }


    template<typename OtherBoard, typename... OtherArgs>
    void setMem(BaseArray<T, OtherBoard> & oArray, OtherArgs... args)
    {
        accessArray<WRITE, OtherBoard, OtherArgs...>(oArray, args...);
    }

    template<typename OtherBoard, typename... OtherArgs>
    void copyTo(BaseArray<T, OtherBoard> & oArray, OtherArgs... args)
    {
        accessArray<READ, OtherBoard, OtherArgs...>(oArray, args...);
    }


    template<typename... OtherArgs>
    void setValue(
        T value,
        OtherArgs... args
    ){
    	recusiveSetValue(
            value,
            0, space_.lastCompleteDim(),
            space_.dimOffset(), space_.nbRangedElement(), 0,
            args...
        );
    }

    size_t offset() const
    {
    	return space_.offset();
    }

    T* ptr() const
    {
		return container_->ptr() + offset();
    }

    operator T*() const
	{
		return ptr();
	}

    template<typename... OtherArgs>
	T getValue(OtherArgs... args) const
	{
		T res, *p(&res);
		memCopy<T*, Board>(p, container_->board_, size_t(1), size_t(0), offset(), args...);
		return res;
	}

    operator T() const
	{
    	return getValue();
	}

    friend std::ostream& operator<<(std::ostream& s, const BaseArray<T, Board>& ba) {
        return s << ba.getValue();
    }

    std::vector<size_t> shape() const
    {
    	return space_.shape();
    }

    size_t nbElement() const
    {
    	return prod(space_.shape());
    }

protected:

    template<u8 Op, typename... OtherArgs>
    void accessArray(
        T * array, OtherArgs... args
    ){
		size_t arrayOffset(0);

        recusiveAccessArray<Op, OtherArgs...>(
            array, arrayOffset,
            0, space_.lastCompleteDim(),
            space_.dimOffset(), space_.nbRangedElement(), 0,
            args...
        );
    }

    template<u8 Op, typename OtherBoard, typename... OtherArgs>
    void accessArray(
        BaseArray<T, OtherBoard> & oArray, OtherArgs... args
    ){
        if(space_ != oArray.space_)
            return;

        recusiveAccessArray<Op, OtherBoard, OtherArgs...>(
            oArray,
            0,
            std::max(space_.nbNotRangedDim(), oArray.space_.nbNotRangedDim()),
            std::min(space_.nbRangedElement(), oArray.space_.nbRangedElement()),
            space_, oArray.space_,
            args...
        );
    }


    template<typename... OtherArgs>
    void recusiveSetValue(
        T value,
        size_t dim, size_t dimStop,
        size_t dimOffset, size_t length, size_t offset,
        OtherArgs... args
    ){
        if(dim < dimStop)
        	for(size_t pos(space_.start_[dim]); pos < space_.stop_[dim]; ++pos)
        		recusiveSetValue(
                    value,
                    dim + 1, dimStop,
                    dimOffset, length, offset * space_.dimAt(dim) + pos,
                    args...
                );
        else
        	memFill(container_->board_, value, length, offset * prod(space_.dim_, dim) + dimOffset, args...);
    }

    template<u8 Op, typename... OtherArgs>
    void recusiveAccessArray(
        T * array, size_t & arrayOffset,
        size_t dim, size_t dimStop, size_t dimOffset,
        size_t length, size_t offset,
        OtherArgs... args
    ){
        if(dim < dimStop)
            for(size_t pos(space_.start_[dim]); pos < space_.stop_[dim]; ++pos)
                recusiveAccessArray<Op, OtherArgs...>(
                    array, arrayOffset,
                    dim + 1, dimStop,
                    dimOffset, length, offset * space_.dimAt(dim) + pos,
                    args...
                );
        else
        {
            if(Op == WRITE)
                memCopy(container_->board_, array, length, offset * prod(space_.dim_, dim) + dimOffset, arrayOffset, args...);
            else
                memCopy(array, container_->board_, length, arrayOffset, offset * prod(space_.dim_, dim) + dimOffset, args...);

            arrayOffset += length;
        }
    }

    template<u8 Op, typename OtherBoard, typename... OtherArgs>
    void recusiveAccessArray(
        BaseArray<T, OtherBoard> & oArray,
        size_t dim, size_t dimStop, size_t length,
        Space space, Space oSpace,
        OtherArgs... args
    ){
        if(dim < dimStop)
            for(size_t pos(0); pos < space.currentLength(); ++pos)
                recusiveAccessArray<Op, OtherBoard, OtherArgs...>(
                    oArray,
                    dim + 1, dimStop, length,
                    space[pos], oSpace[pos],
                    args...
                );
        else
        {
            if(Op == WRITE)
                memCopy(container_->board_, oArray.container_->board_, length, space.offset(), oSpace.offset(), args...);
            else
                memCopy(oArray.container_->board_, container_->board_, length, oSpace.offset(), space.offset(), args...);
        }
    }

    //Contain the data
    std::shared_ptr<Container<T, Board>> container_;
    //Define current space
    Space space_;
};


template<typename T>
struct Host
{
    Host():ptr_(nullptr){}

    Host(size_t const nbElement):ptr_(nullptr)
    {
        allocate(nbElement);
    }

    ~Host()
    {
    	deallocate();
    }

    void allocate(size_t const nbElement)
    {
        if(ptr_)
        	deallocate();

        ptr_ = new T[nbElement];

    }

    void deallocate()
    {
        if(ptr_)
        {
            delete[] ptr_;
            ptr_ = nullptr;
        }
    }

    operator T*() const
    {
        return ptr_;
    }

    T* operator+(size_t const offset) const
    {
    	return ptr_ + offset;
    }

    T* ptr_;
};


template<typename T1, typename T2>
void memCopy(T1 & dst, T2 const & src, size_t nbElement, size_t offsetDst, size_t offsetSrc)
{
    std::copy(src + offsetSrc, src + offsetSrc + nbElement, dst + offsetDst);
}

template<typename T>
void memFill(Host<T> & dst, T value, size_t nbElement, size_t offsetDst)
{
    std::fill(dst + offsetDst, dst + offsetDst + nbElement, value);
}

template<typename T>
using Array = BaseArray<T, Host<T>>;
