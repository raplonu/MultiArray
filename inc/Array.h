#include <iostream>
#include<string>
#include <memory>
#include <array>
#include <vector>
#include <algorithm>
#include <typeinfo>


using u8 = char;

u8 const
    READ(0),
    WRITE(1);

void handleError(
	std::string err,
	char const * file,
	int line
){
	std::cerr << "Array Error : " << err << " in " << file << " at line " << line << std::endl;
	exit( EXIT_FAILURE );
}

#define HANDLE_ERROR(err) (handleError((err), __FILE__, __LINE__ ))

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

template<typename T>
static size_t prod(std::vector<T> const & vect, size_t offset = 0)
{
    size_t res(1);
    for(size_t i(offset); i < vect.size(); ++i)
        res *= vect[i];
    return res;
}

template<typename T, typename Board>
class Container
{
public:
	size_t _length;
    Board _board;

    Container():
        _length(0),
        _board()
    {}

	template<typename... OtherArgs>
    Container(size_t length, OtherArgs... args):
        _length(length),
        _board(length, args...)
    {}

    ~Container(){}

    T* ptr() const
    {
        return _board.ptr();
    }

    size_t length() const
    {
        return _length;
    }

    size_t size() const
    {
        return _length * sizeof(T);
    }
};

struct Dimension
{
	size_t dim;
	size_t start;
	size_t stop;

    Dimension(){}

    Dimension(size_t length)
    :dim(length), start(0), stop(length)
    {}

    Dimension(size_t dim, size_t start, size_t stop)
    :dim(dim), start(start), stop(stop)
    {}

    Dimension(Dimension const & oD) = default;
    Dimension(Dimension && oD) = default;

    Dimension & operator=(Dimension const & oD) = default;
    Dimension & operator=(Dimension && oD) = default;

    ~Dimension(){}

    void reset()
    {
        start = 0;
        stop = dim;
    }

    bool isActive() const
    {
        return stop > start;
    }

    bool isComplete() const
    {
        return (start == 0) && (stop == dim);
    }

    size_t length() const
    {
        return stop - start;
    }

    void updateOffset(size_t & offset) const
    {
        offset = offset * dim + start;
    }
};

class Space
{
    std::vector<Dimension> _dims;

public:
    Space():
        _dims()
    {}

    Space(size_t length):
        _dims(1, Dimension(length))
    {}

    template<typename A, size_t NbDim>
    Space(A const (& lengths)[NbDim]):
        _dims(NbDim)
    {
    	for(size_t i(0); i < NbDim; ++i)
            _dims[i] = Dimension(lengths[i]);
    }

    Space(std::vector<size_t> const & lengths):
        _dims(lengths.size())
    {
    	for(size_t i(0); i < lengths.size(); ++i)
            _dims[i] = Dimension(lengths[i]);
    }

    Space(
        std::vector<Dimension> const & dims
    ):_dims(dims)
    {}

    Space(
        std::vector<Dimension> && dims
    ):_dims(std::move(dims))
    {}

    Space(Space const & bs):
        _dims(bs._dims)
    {}

    Space(Space && bs):
        _dims(std::move(bs._dims))
    {}

    Space & operator=(Space const & bs)
    {
        _dims = bs._dims;
        return *this;
    }

    Space & operator=(Space && bs)
    {
        _dims = std::move(bs._dims);
        return *this;
    }

    ~Space(){}

    Space operator[](size_t pos) const
    {
        size_t changeDim(firstActiveDim());
        std::vector<Dimension> newDims(_dims);
        Dimension & d(newDims[changeDim]);

        d.start += pos;
        d.stop = d.start;

        return Space(std::move(newDims));
    }

    Space at(
        std::vector<size_t> const & start,
        std::vector<size_t> const & stop
    ) const
    {
        //Note how many dimension we have
        size_t nbActiveD(nbActiveDim());

        //Test if space have enought dimension for args
        if(start.size() != stop.size())
            HANDLE_ERROR("start & stop don't have same size");

        if(start.size() > nbActiveD)
            HANDLE_ERROR("Not enought active dims");

        //Create new dimensions
        std::vector<Dimension> newDims(_dims);

        //Create 2 iterators, one for space dimensions, other for args dimensions
        size_t
            dimsIndex(firstActiveDim()),
            argsIndex(0);

        //For each argsIndex dimensions
        for(; argsIndex < nbActiveD; ++dimsIndex)
        {
            //Only compute if current dim is active
            if(isActive(dimsIndex))
            {
                Dimension & d = newDims[dimsIndex];

                d.stop = d.start + stop[argsIndex];
                d.start += start[argsIndex];

                if(d.start >= d.dim)
                    HANDLE_ERROR("start dim bigger than dim size");
                if(d.stop > _dims[dimsIndex].stop)
                    HANDLE_ERROR("Can't enlarge stop limit that way !");

                argsIndex++;
            }
        }

        return Space(std::move(newDims));
    }

    Space at(
        size_t start,
        size_t stop
    ) const
    {
        //Note how many dimension we have
        size_t dim(firstActiveDim());

        //Create new dimensions
        std::vector<Dimension> newDims(_dims);
        Dimension & d(newDims[dim]);

        d.stop = d.start + stop;
        d.start += start;

        if(d.start >= d.dim)
            HANDLE_ERROR("start dim bigger than dim size");
        if(d.stop > _dims[dim].stop)
            HANDLE_ERROR("Can't enlarge stop limit that way !");

        return Space(std::move(newDims));
    }

    Space base() const
    {
        std::vector<Dimension> newDims(_dims);

        for(size_t d(0); d < nbDim(); ++d)
            newDims[d].reset();

        return Space(std::move(newDims));
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
    // std::vector<size_t> const & dim() const
    // {
    //     return dim_;
    // }

    //Return the current dim of data
    std::vector<size_t> shape() const
    {
        std::vector<size_t> shape(nbActiveDim());

        size_t j(0);
        for(size_t i(0); i < nbDim(); ++i)
            if(isActive(i))
                shape[j++] = lengthAt(i);

        return shape;
    }

    Dimension dimensionAt(size_t dim) const
    {
        if(dim < nbDim())
            return _dims[dim];
        else
            return 0;
    }

    //The dimension size @ dim
    size_t dimAt(size_t dim) const
    {
        return dimensionAt(dim).dim;
    }

    //The current dim @ dim
    size_t lengthAt(size_t dim) const
    {
        return _dims[dim].length();
    }

    //The current dim @ dim
    size_t realLengthAt(size_t dim) const
    {
        return std::max(_dims[dim].length(), size_t(1));
    }

    //Return the current dim @ dim active dimension
    size_t shapeAt(size_t dim) const
    {
        size_t j(0);

        for(size_t i(0); i < nbDim(); ++i)
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
        return _dims.size();
    }

    size_t nbActiveDimUntil(size_t maxDim) const
    {
        size_t activeDim(0);

        for(size_t i(0); i < maxDim; ++i)
            if(isActive(i))
                activeDim++;

        return activeDim;
    }

    size_t nbActiveDim() const
    {
        return nbActiveDimUntil(nbDim());
    }

    bool isActive(size_t dim) const
    {

        return _dims[dim].isActive();
    }

    bool isDimComplete(size_t dim) const
    {
        return _dims[dim].isComplete();
    }

    size_t offsetUntil(size_t dim) const
    {
        size_t offset(_dims[0].start);

        for(size_t d(1); d < dim; ++d)
            _dims[d].updateOffset(offset);

        return offset;
    }

    size_t offset() const
    {
        return offsetUntil(nbDim());
    }

    size_t dimOffset() const
    {
        size_t dim(nbDim() - 1), offset(1);

        while(dim > 0 && isDimComplete(dim))
            offset *= _dims[dim--].dim;

        return offset * _dims[dim].start;
    }

    size_t dimProd(size_t dim) const
    {
        size_t offset(_dims[0].dim);

        for(size_t d(1); d < dim; ++d)
            offset *= _dims[d].dim;

        return offset;
    }

    size_t nbRangedElement() const
    {
        size_t dim(nbDim() - 1), range(realLengthAt(dim));

        while(dim > 0 && isDimComplete(dim--))
            range *= realLengthAt(dim);

        return range;
    }

    size_t nbNotRangedDim() const
    {
        size_t dim(nbDim() - 1), notRangedDim(0);

        while(dim > 0 && isDimComplete(dim--));

        while(dim > 0 && isActive(dim--))
        	++notRangedDim;

        return notRangedDim;
    }

    size_t firstActiveDim() const
    {
        for(size_t i(0); i < nbDim(); ++i)
            if(isActive(i))
                return i;

        return nbDim();
    }

    size_t lastActiveDim() const
    {
        for(size_t i(nbDim() - 1); i > 0; --i)
            if(isActive(i))
                return i;

        return isActive(0);
    }

    bool hasActiveDim() const
    {
        return firstActiveDim() != nbDim();
    }

    bool isLastDimActive() const
    {
        return isActive(nbDim() - 1);
    }

    size_t lastCompleteDim() const
    {
        size_t d(nbDim());

        while(d > 0 && isDimComplete(--d));

        return d;
    }
};

template<typename T, typename Board> class BaseArray;

template<typename T, typename Board>
class BaseArray
{
    //Contain the data
    std::shared_ptr<Container<T, Board>> _container;
    //Define current space
    Space _space;

public:
    //Void constructor
    BaseArray():
        _container(nullptr),
        _space()
    {}

    //1 dimensions constructor with init array and optionnal board specific args
    template<typename... OtherArgs>
    BaseArray(size_t length, T * array, OtherArgs... args):
        _container(std::make_shared<Container<T, Board>>(length, args...)),
        _space(length)
    {
        setMem(array);
    }

    //N dimensions constructor with init array and optionnal board specific args
    template<typename A, size_t NbDim, typename... OtherArgs>
    BaseArray(A const (&&lengths)[NbDim], T * array, OtherArgs... args):
        _container(std::make_shared<Container<T, Board>>(prod(lengths), args...)),
        _space(lengths)
    {
        setMem(array);
    }

    //1 dimensions constructor with init value and optionnal board specific args
    template<typename... OtherArgs>
    BaseArray(size_t length, T value, OtherArgs... args):
        _container(std::make_shared<Container<T, Board>>(length, args...)),
        _space(length)
    {
        fill(value);
    }

    //N dimensions constructor with init value and optionnal board specific args
    template<typename A, size_t NbDim, typename... OtherArgs>
    BaseArray(A const (&&lengths)[NbDim], T value, OtherArgs... args):
        _container(std::make_shared<Container<T, Board>>(prod(lengths), args...)),
        _space(lengths)
    {
        fill(value);
    }

    //1 dimensions constructor with optionnal board specific args
	template<typename... OtherArgs>
    BaseArray(size_t length, OtherArgs... args):
        _container(std::make_shared<Container<T, Board>>(length, args...)),
        _space(length)
    {}

    //N dimensions constructor with optionnal board specific args
    template<typename A, size_t NbDim, typename... OtherArgs>
    BaseArray(A const (&&lengths)[NbDim], OtherArgs... args):
        _container(std::make_shared<Container<T, Board>>(prod(lengths), args...)),
        _space(lengths)
    {}

    BaseArray(
        std::shared_ptr<Container<T, Board>> container,
        Space space
    ):
        _container(std::move(container)),
        _space(std::move(space))
    {}

    BaseArray(
    	BaseArray<T, Board> & ob
    ):
        _container(ob._container),
        _space(ob._space)
    {}

    BaseArray(
    	BaseArray<T, Board> && ob
    ):
        _container(std::move(ob._container)),
        _space(std::move(ob._space))
    {}

    ~BaseArray(){}

    //Copy
    BaseArray<T, Board> & operator=(BaseArray<T, Board> & oArray)
    {
    	if(_container)
    	{
    		accessArray<WRITE, Board>(oArray);
    	}
    	else
    	{
    		//Case with no _container
    		_container = oArray._container;
    		_space = oArray._space;
    	}
    	return *this;
    }

    BaseArray<T, Board> & operator=(BaseArray<T, Board> && oArray)
    {
    	if(_container)
    	{
    		accessArray<WRITE, Board>(oArray);
    	}
    	else
    	{
    		//Case with no _container
            _container = std::move(oArray._container);
    		_space = std::move(oArray._space);
    	}
    	return *this;
    }

    template<typename OtherBoard>
    BaseArray<T, Board> & operator=(BaseArray<T, OtherBoard> & oArray)
    {
        if(!_container)
        {
            _container = std::make_shared<Container<T, Board>>(oArray.length());//oArray._container);
    		_space = oArray._space;
        }

    	accessArray<WRITE, OtherBoard>(oArray);

    	return *this;
    }

    template<typename OtherBoard>
    BaseArray<T, Board> & operator=(BaseArray<T, OtherBoard> && oArray)
    {
        if(!_container)
        {
            _container = std::make_shared<Container<T, Board>>(oArray.length());
            _space = std::move(oArray._space);
        }

    	accessArray<WRITE, OtherBoard>(oArray);
    	return *this;
    }

    template<typename A, size_t NbDim>
    BaseArray<T, Board> at(
    	A const (&&start)[NbDim],
    	A const (&&stop)[NbDim]
    ) const
    {
        return BaseArray<T, Board>(_container, std::move(_space.at(std::move(start), std::move(stop))));
    }

    BaseArray<T, Board> at(size_t start, size_t stop) const
    {
        return BaseArray<T, Board>(_container, std::move(_space.at(start, stop)));
    }

    BaseArray<T, Board> operator[](size_t pos) const
    {
        return BaseArray<T, Board>(_container, std::move(_space[pos]));
    }

    BaseArray<T, Board> base() const
	{
    	return BaseArray<T, Board>(_container, std::move(_space.base()));
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
    	fill(value, args...);
    }

    size_t offset() const
    {
    	return _space.offset();
    }

    T* ptr() const
    {
		return _container->ptr() + offset();
    }

    operator T*() const
	{
		return ptr();
	}

    template<typename... OtherArgs>
	T getValue(OtherArgs... args) const
	{
		T res, *p(&res);

		memCopy<T*, Board>(p, _container->_board, size_t(1), size_t(0), offset(), args...);

		return res;
	}

    operator T() const
	{
    	return getValue();
	}

    // friend std::ostream& operator<<(std::ostream& s, const BaseArray<T, Board>& ba) {
    //     return s << ba.getValue();
    // }

    std::vector<size_t> shape() const
    {
    	return _space.shape();
    }

    size_t length() const
    {
    	return prod(_space.shape());
    }

protected:

    template<typename... OtherArgs>
    void fill(
        T value,
        OtherArgs... args
    ){
    	recusiveSetValue(
            value,
            0, _space.lastCompleteDim(),
            _space.dimOffset(), _space.nbRangedElement(), 0,
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
        Dimension d(_space.dimensionAt(dim));

        if(dim < dimStop)
        {
            size_t pos(d.start);
            do
            {
        		recusiveSetValue(
                    value,
                    dim + 1, dimStop,
                    dimOffset, length, offset * d.dim + pos,
                    args...
                );
            }
            while(pos++ < d.stop);
        }
        else
        	memFill(_container->_board, value, length, offset * _space.dimProd(dim) + dimOffset, args...);
    }

    template<u8 Op, typename... OtherArgs>
    void accessArray(
        T * array, OtherArgs... args
    ){
		size_t arrayOffset(0);

        recusiveAccessArray<Op, OtherArgs...>(
            array, arrayOffset,
            0, _space.lastCompleteDim(),
            _space.dimOffset(), _space.nbRangedElement(), 0,
            args...
        );
    }

    template<u8 Op, typename OtherBoard, typename... OtherArgs>
    void accessArray(
        BaseArray<T, OtherBoard> & oArray, OtherArgs... args
    ){
        if(_space != oArray._space)
            HANDLE_ERROR("Dimensions don't compute");

        recusiveAccessArray<Op, OtherBoard, OtherArgs...>(
            oArray,
            0,
            std::max(_space.nbNotRangedDim(), oArray._space.nbNotRangedDim()),
            std::min(_space.nbRangedElement(), oArray._space.nbRangedElement()),
            _space, oArray._space,
            args...
        );
    }

    template<u8 Op, typename... OtherArgs>
    void recusiveAccessArray(
        T * array, size_t & arrayOffset,
        size_t dim, size_t dimStop, size_t dimOffset,
        size_t length, size_t offset,
        OtherArgs... args
    ){
        Dimension d(_space.dimensionAt(dim));

        if(dim < dimStop)
        {
            size_t pos(d.start);
            do
            {
                recusiveAccessArray<Op, OtherArgs...>(
                    array, arrayOffset,
                    dim + 1, dimStop,
                    dimOffset, length, offset * d.dim + pos,
                    args...
                );
            } while (pos++ < d.stop);
        }
        else
        {
            if(Op == WRITE)
                memCopy(_container->_board, array, length, offset * _space.dimProd(dim) + dimOffset, arrayOffset, args...);
            else
                memCopy(array, _container->_board, length, arrayOffset, offset * _space.dimProd(dim) + dimOffset, args...);

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
                memCopy(_container->_board, oArray._container->_board, length, space.offset(), oSpace.offset(), args...);
            else
                memCopy(oArray._container->_board, _container->_board, length, oSpace.offset(), space.offset(), args...);
        }
    }
};


template<typename T>
struct Host
{
    T* _ptr;

    Host():_ptr(nullptr){}

    Host(size_t length):_ptr(nullptr)
    {
        allocate(length);
    }

    ~Host()
    {
    	deallocate();
    }

    void allocate(size_t length)
    {
        if(_ptr)
        	deallocate();

        _ptr = new T[length];

    }

    void deallocate()
    {
        if(_ptr)
        {
            delete[] _ptr;
            _ptr = nullptr;
        }
    }

    operator T*() const
    {
        return _ptr;
    }

    T* operator+(size_t offset) const
    {
    	return _ptr + offset;
    }
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

template<typename T>
std::ostream& operator<<(std::ostream& s, const Array<T>& a) {
    auto shape = a.shape();
    char comma[3] = {'\0', ' ', '\0'};

    if(shape.size() > 1)
        for (size_t i(0); i < shape[0]; ++i)
            s << a[i] << std::endl;
    else
    {
        s.put('[');
        for (size_t i(0); i < shape[0]; ++i)
        {
            s << comma << a[i].getValue();
            comma[0] = ',';
        }
        s << ']';
    }
    return s;
}
