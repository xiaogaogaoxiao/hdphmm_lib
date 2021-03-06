// file: $isip/class/dstr/DoubleLinkedList/DoubleLinkedList.h
// version: $Id: DoubleLinkedList.h 10640 2007-01-27 02:36:04Z tm334 $
//

// make sure definitions are made only once
//
#ifndef ISIP_DOUBLE_LINKED_LIST
#define ISIP_DOUBLE_LINKED_LIST

// include files
//
#include <typeinfo>

// isip include files
//
#ifndef ISIP_DSTR_BASE
#include <DstrBase.h>
#endif

#ifndef ISIP_DOUBLE_LINKED_NODE
#include <DoubleLinkedNode.h>
#endif

#ifndef ISIP_VECTOR_LONG
#include <VectorLong.h>
#endif

#ifndef ISIP_STRING
#include <String.h>
#endif

#ifndef ISIP_CHAR
#include <Char.h>
#endif

#ifndef ISIP_LONG
#include <Long.h>
#endif

#ifndef ISIP_CONSOLE
#include <Console.h>
#endif

// forward class definitions
//
template<class TObject> class DoubleLinkedListDiagnose;

// DoubleLinkedList: a generic doubly-linked list template class. the
// user may choose whether or not the DoubleLinkedList is used in
// USER-allocated or SYSTEM-allocated mode. in SYSTEM-allocated mode,
// the DoubleLinkedList class is responsible for managing the memory
// for the objects it contains. this implies that a copy is made each
// time an object is inserted to the list. in USER-allocation mode the
// user is responsible for allocation and deletion of the objects that
// are stored in the DoubleLinkedList. in this mode, pointers to the
// external objects are stored, i.e. a copy is not made. the mode
// chosen is dependent on the user's need to trade-off speed versus
// complexity of object management.
//
template<class TObject>
class DoubleLinkedList : public DstrBase {
  
  //---------------------------------------------------------------------------
  //
  // public constants
  //
  //---------------------------------------------------------------------------
public:
  
  // define the class name
  //
  static const String CLASS_NAME;
  
  //----------------------------------------
  //
  // i/o related constants
  //
  //----------------------------------------  
  
  static const String DEF_PARAM;
  
  //----------------------------------------
  //
  // default values and arguments
  //
  //----------------------------------------
  
  // default values
  //

  // default arguments to methods
  //
  
  //----------------------------------------
  //
  // error codes
  //
  //----------------------------------------  
  
  static const int32 ERR = 40400;

//----------------------------------------
//
// Inner Stack class for randQuickSort()
//
//----------------------------------------  
template<class TObject2>
class Stack : public DstrBase {

public:
  static const int32 POP_ALL = -1;
  static const int32 POP_TO_MARK = -2;
  static const int32 ERR = 40500;
  static const int32 ERR_EMPTY = 40501;
  
protected:
  
  // the internal structure of the stack is a DoubleLinkedList
  //
  DoubleLinkedList<TObject2> stack_d;

  // the allocation mode
  //
  ALLOCATION alloc_d;

  // define the memory manager
  //
  static MemoryManager mgr_d;
  
  // debugging parameters
  //
  static Integral::DEBUG debug_level_d;
  
public:

  // method: name
  //
  static const String& name() {
    // create the static name string for this class and return it
    //
    static String cname(L"Stack");
    cname.clear();
    cname.concat(L"Stack");
    cname.concat(L"<");
    cname.concat(TObject2::name());
    cname.concat(L">");
  
    // return the name
    //
    return cname;
  }

  // method: setDebug
  //
  static bool8 setDebug(Integral::DEBUG debug_level) {
    debug_level_d = debug_level;
    return true;
  }

  // other debug methods:
  //
  bool8 debug(const unichar* message_a) const {

    // declare local variables
    //
    String output;
    String value;

    // print out the memory allocation mode
    //
    output.debugStr(name(), message_a, L"alloc_d",
  		  NameMap::ALLOCATION_MAP((int32)alloc_d));
    Console::put(output);

    // output the debug level
    //
    value.assign(debug_level_d);
    output.debugStr(name(), message_a, L"debug_level_d", value);
    Console::put(output);
  
    // print out the top of stack message
    //
    value.assign(L"top of stack");
    Console::put(value);
  
    // increment the indention level
    //
    Console::increaseIndention();
  
    // call the list debug method
    //
    stack_d.debug(message_a);
    
    // decrement the indention level
    //
    Console::decreaseIndention();
  
    // exit gracefully
    // 
    return true;
  }

  // method: destructor
  //
  ~Stack() {
    clear(Integral::RESET);
  }
  
  // default constructor
  //
  Stack(ALLOCATION alloc_a = DEF_ALLOCATION) {

    // set the allocation flag
    //
    alloc_d = alloc_a;
    
    // the list object must be in USER mode
    //
    stack_d.setAllocationMode(USER);
  }
  
  // method: copy constructor
  //
  Stack(const Stack<TObject2>& copy_stack) {
    alloc_d = copy_stack.alloc_d;
    stack_d.setAllocationMode(USER);
    assign(copy_stack);
  }
  
  // assign method
  //
  bool8 assign(const Stack<TObject2>& copy_stack_a) {
  
    // error when the current stack is in USER mode and the stack to be copied
    // is in SYSTEM mode
    //
    if ((alloc_d == USER) && (copy_stack_a.getAllocationMode() == SYSTEM)) {
      return Error::handle(name(), L"assign", Error::ARG, __FILE__, __LINE__);
    }
  
    // if this stack is in system mode then temporarily set the list
    // to system mode and assign. this has the side-effect of copying the
    // data to this list
    //
    alloc_d = copy_stack_a.alloc_d;
    stack_d.setAllocationMode(alloc_d);
    stack_d.assign(copy_stack_a.stack_d);
    stack_d.setAllocationMode(USER);
    
    // reposition the stack
    //
    stack_d.gotoFirst();
  
    // exit gracefully
    //
    return true;
  }

  // method: operator=
  //
  Stack<TObject2>& operator=(const Stack<TObject2>& arg) {
    assign(arg);
    return *this;
  }

  // method: eq
  //
  bool8 eq(const Stack<TObject2>& compare_stack) const {
    return stack_d.eq(compare_stack.stack_d);
  }
  
  // method: sofSize
  //
  int32 sofSize() const {
    return stack_d.sofSize();
  }

  // method: read
  //
  bool8 read(Sof& sof, int32 tag) {
    return read(sof, tag, name());
  }

  // method: write
  //
  bool8 write(Sof& sof, int32 tag) const {
    return write(sof, tag, name());
  }

  // method: read
  //
  bool8 read(Sof& sof, int32 tag, const String& name) {
    return stack_d.read(sof, tag, name);
  }
  
  // method: write
  //
  bool8 write(Sof& sof, int32 tag, const String& name) const {
    return stack_d.write(sof, tag, name);
  }

  // method: readData
  //
  bool8 readData(Sof& sof, const String& pname = DEF_PARAM, int32 size =
		   SofParser::FULL_OBJECT, bool8 param = true,
		   bool8 nested = false) {
    return stack_d.readData(sof, pname, size, param, nested);
  }

  // method: writeData
  //
  bool8 writeData(Sof& sof, const String& pname =  DEF_PARAM) const {
    return stack_d.writeData(sof, pname);
  }
  
  // method: new
  //
  static void* operator new(size_t size) {
    return mgr_d.get();
  }

  // method: new[]
  //
  static void* operator new[](size_t size) {
    return mgr_d.getBlock(size);
  }    

  // method: delete
  //
  static void operator delete(void* ptr) {
    mgr_d.release(ptr);
  }

  // method: delete[]
  //  
  static void operator delete[](void* ptr) {
    mgr_d.releaseBlock(ptr);
  }

  // method: setGrowSize
  // 
  static bool8 setGrowSize(int32 grow_size) {
    return mgr_d.setGrow(grow_size);
  }

  // method: clear
  //  
  bool8 clear(Integral::CMODE cmode = Integral::DEF_CMODE) {
    stack_d.setAllocationMode(alloc_d);
    bool8 ret = stack_d.clear(cmode);
    stack_d.setAllocationMode(USER);
    return ret;
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  extensions to required methods
  //
  //---------------------------------------------------------------------------

  // method: ne
  //
  bool8 ne(const Stack<TObject2>& compare_stack) const {
    return (!eq(compare_stack));
  }
  
  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  stack push and pop methods
  //
  //---------------------------------------------------------------------------
  
  // push methods:
  //  the push methods add items to the top of the stack
  //
  bool8 push(TObject2* item_a) {
  
    // declare local variables
    //
    TObject2* new_obj = (TObject2*)NULL;
  
    // mode: SYSTEM
    //  make a copy of the element
    //
    if (alloc_d == SYSTEM) {
      new_obj = new TObject2(*item_a);
    }
  
    // mode: USER
    //  make a copy of the pointer to the item
    //
    else {
      new_obj = item_a;
    }
  
    // push the object to the top of the stack
    //
    return stack_d.insertFirst(new_obj);
  }

  // other stack manipulation methods
  //
  const TObject2* peek() const  {
      return stack_d.getCurr();
  }

  TObject2* peek() {
      return stack_d.getCurr();
  }


  TObject2* pop(TObject2* item_a = (TObject2*)NULL) {
  
    // declare local variables
    //
    bool8 result = false;
    TObject2* ptr = (TObject2*)NULL;
  
    // when in SYSTEM mode the item passed in should not be
    // null, if we are in reference mode the item should be null
    //
    if (((alloc_d == SYSTEM) && (item_a == (TObject2*)NULL)) ||
        ((alloc_d == USER) && (item_a != (TObject2*)NULL))) {
      Error::handle(name(), L"pop", ERR, __FILE__, __LINE__);
      return (TObject2*)NULL;
    }
  
    // when in USER mode the item should be null
    //
    if ((alloc_d == USER) && (item_a != (TObject2*)NULL)) {
      Error::handle(name(), L"pop", ERR, __FILE__, __LINE__);
      return (TObject2*)NULL;
    }  
    
    // if the stack is empty exit
    //
    if (!isEmpty()) {
  
      // the returned item is valid so remove the item and make sure we are
      // positioned at the top of the stack
      //
      result = stack_d.removeFirst(ptr);
      stack_d.gotoFirst();
      
      // mode: SYSTEM
      //
      if (alloc_d == SYSTEM) {
  
        // make a copy of the object
        //
        item_a->assign(*ptr);
  
        // delete the reference to the object      
        //
        delete ptr;
  
        // return a pointer to the object (so that the return value can
        // be compared).
        //
        return item_a;
      }
  
      // mode: USER
      //
      else {
        return ptr;
      }
    }
  
    // nothing in stack, return null
    //
    return (TObject2*)NULL;
  }

  // method: popAll
  //
  bool8 popAll(Stack<TObject2>& item_stack) {
    if (pop(item_stack, POP_ALL) <= 0) {
      return false;
    }
    return true;
  }

  // method: popToMark
  //
  bool8 popToMark(Stack<TObject2>& item_stack) {
    if (pop(item_stack, POP_TO_MARK) <= 0) {
      return false;
    }
    return true;
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  stack marker positioning methods
  //
  //---------------------------------------------------------------------------
  
  // method: setMark
  //  sets the top of the stack to be the marked position
  //
  bool8 setMark() {
    return stack_d.setMark();
  }

  // method: clearMark
  //  unmarks the marked stack position
  //
  bool8 clearMark() {
    return stack_d.clearMark();
  }

  // method: markIsSet
  //
  bool8 markIsSet() const {
    return stack_d.markIsSet();
  }

  // method: getMark
  //
  const TObject2* getMark() const {
    return stack_d.getMark();
  }

  // method: getMark
  //
  TObject2* getMark() {
    return stack_d.getMark();
  }

  // method: isMarkedElement
  //  indicates if the marked element is on top of the stack
  //
  bool8 isMarkedElement() const {
    return stack_d.isMarkedElement();
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  stack property methods
  //
  //---------------------------------------------------------------------------
  
  // method: isEmpty
  //
  bool8 isEmpty() const {
    return (stack_d.isEmpty());
  }

  // method: length
  //
  int32 length() const {
    return stack_d.length();
  }

  // method: find
  //
  bool8 find(TObject2* item) {

    // since the position of the stack must be at the top of the
    // stack, we set the mark to the element if it is found
    //
    return (stack_d.find(item) && stack_d.setMark() &&
	    stack_d.gotoFirst());
  }

  // method: contains
  //
  bool8 contains(TObject2* item) const {
    return stack_d.contains(item);
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  stack computation methods
  //
  //---------------------------------------------------------------------------
    
  // method: apply
  //  apply an external function to each element in the list
  //
  bool8 apply(bool8 (TObject2::*method)()) {
    return (stack_d.apply(method));
  }

  // method: apply
  //  applies the input method to each element in the input stack
  //  and assigns the input stack to this stack
  //
  bool8 apply(bool8 (TObject2::*method)(), Stack<TObject2>& arg) {
    assign(arg);
    stack_d.gotoFirst();
    return apply(method);
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  stack element ordering methods
  //
  //---------------------------------------------------------------------------
  
  // method: sort
  //  sorts the elements in the stack according to the given order with
  //  ascending order, the item at the bottom of the stack will be the
  //  greatest in value.
  //
  bool8 sort(Integral::ORDER sort_order = Integral::ASCENDING,
               DstrBase::SORT_ALGO sort_algo =  DstrBase::DEF_SORT_ALGO) {
    return stack_d.sort(sort_order, sort_algo);
  }

  // method: reverse
  //  reverses the order of elements in the stack
  //
  bool8 reverse() {
    return stack_d.reverse();
  }
      
  // method: getAllocationMode
  //
  ALLOCATION getAllocationMode() const {
    return alloc_d;
  }

  // method: setAllocationMode
  //
  bool8 setAllocationMode(ALLOCATION alloc) {
    alloc_d = alloc;
    return true;
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  stack container access methods
  //
  //---------------------------------------------------------------------------

  // method: gotoFirst
  //
  bool8 gotoFirst() {
    return stack_d.gotoFirst();
  }

  // method: gotoNext
  //  
  bool8 gotoNext() {
    return stack_d.gotoNext();    
  }

  // method: gotoPrev
  //    
  bool8 gotoPrev() {
    return stack_d.gotoPrev();
  }

  // method: gotoMark
  //    
  bool8 gotoMark() {
    return stack_d.gotoMark();
  }  

  // method: getFirst
  //
  TObject2* getFirst() {
    return stack_d.getFirst();
  }

  // method: getNext
  //  
  TObject2* getNext() {
    return stack_d.getNext();    
  }

  // method: getPrev
  //    
  TObject2* getPrev() {
    return stack_d.getPrev();
  }

  // method: getCurr
  //    
  TObject2* getCurr() {
    return stack_d.getCurr();
  }  

  // method: gotoPosition
  //      
  bool8 gotoPosition() {
    return stack_d.gotoPosition();
  }

  // method: getPosition
  //        
  int32 getPosition() {
    return stack_d.getPosition();
  }

  // method: isLast
  //
  bool8 isLast() {
    return stack_d.isLast();
  }

  // method: isEmpty
  //
  bool8 isEmpty() {
    return stack_d.isEmpty();
  }

};//end of inner class Stack
  
  //---------------------------------------------------------------------------
  //
  // protected data
  //
  //---------------------------------------------------------------------------
protected:

  // define the NODE object
  //
  typedef DoubleLinkedNode<TObject> NODE;
  
  // pointers to the first node, last node and current node
  //
  NODE* first_d;
  NODE* last_d; 
  NODE* curr_d;
  
  // a pointer to a node the user marks
  //
  NODE* mark_d;
  
  // the number of nodes in the list
  //
  Long length_d;

  // the allocation mode
  //
  ALLOCATION alloc_d;

  // debugging parameters
  //
  static Integral::DEBUG debug_level_d;
  
  // define the memory manager
  //
  static MemoryManager mgr_d;
  
  //---------------------------------------------------------------------------
  //
  // required public methods
  //
  //---------------------------------------------------------------------------
public:
  
  // static methods:
  //  diagnose method is moved outside the class header file and
  //  defined in the DoubleLinkedListDiagnose.h in order to avoid issues
  //  related to preprocessing of the diagnose code.
  //
  static const String& name();
  
  // method: setDebug
  //
  static bool8 setDebug(Integral::DEBUG debug_level) {
    debug_level_d = debug_level;
    return true;
  }

  // other debug methods
  //
  bool8 debug(const unichar* message) const;

  // method: destructor
  //
  ~DoubleLinkedList() {
    clear(Integral::RESET);
  }

  // default constructor
  //
  DoubleLinkedList(ALLOCATION alloc = DEF_ALLOCATION);

  // copy constructor
  //
  DoubleLinkedList(const DoubleLinkedList<TObject>& copy_list);
  
  // assign methods
  //
  bool8 assign(const DoubleLinkedList<TObject>& copy_list);

  // method: operator=
  //
  DoubleLinkedList<TObject>& operator=(const DoubleLinkedList<TObject>& arg) {
    assign(arg);
    return *this;
  }

  // equality methods
  //
  bool8 eq(const DoubleLinkedList<TObject>& compare_list) const;

  // i/o methods:
  //
  int32 sofSize() const;
  
  // method: read
  //
  bool8 read(Sof& sof, int32 tag) {
    return read(sof, tag, name());
  }

  // method: write
  //
  bool8 write(Sof& sof, int32 tag) const {
    return write(sof, tag, name());
  }

  // other i/o methods
  //
  bool8 read(Sof& sof, int32 tag, const String& name);
  bool8 write(Sof& sof, int32 tag, const String& name) const;

  bool8 readData(Sof& sof, const String& pname = DEF_PARAM,
		   int32 size = SofParser::FULL_OBJECT,
		   bool8 param = true, bool8 nested = false);
  bool8 writeData(Sof& sof, const String& pname = DEF_PARAM) const;

  // method: new
  //
  static void* operator new(size_t size) {
    return mgr_d.get();
  }

  // method: new[]
  //
  static void* operator new[](size_t size) {
    return mgr_d.getBlock(size);
  }

  // method: delete
  //
  static void operator delete(void* ptr) {
    mgr_d.release(ptr);
  }

  // method: delete[]
  //
  static void operator delete[](void* ptr) {
    mgr_d.releaseBlock(ptr);
  }

  // method: setGrowSize
  //
  static bool8 setGrowSize(int32 grow_size) {
    return mgr_d.setGrow(grow_size);
  }

  // other memory management methods
  //
  bool8 clear(Integral::CMODE cmode = Integral::DEF_CMODE);

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  overload operators and extensions to required methods
  //
  //---------------------------------------------------------------------------

  // overloaded operators
  //
  TObject& operator() (int32 index);
  const TObject& operator() (int32 index) const;

  // method: ne
  //
  bool8 ne(const DoubleLinkedList<TObject>& compare_list) const {
    return (!eq(compare_list));
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  positioning methods
  //
  //---------------------------------------------------------------------------

  // method: gotoFirst
  //
  bool8 gotoFirst() {
    return ((curr_d = first_d) != (NODE*)NULL);
  }

  // method: gotoLast
  //
  bool8 gotoLast() {
    return ((curr_d = last_d) != (NODE*)NULL);
  }

  // other positioning methods:
  //  these methods move to the specified node and return a bool8 indicating
  //  whether or not the node is valid
  //
  bool8 gotoNext();
  bool8 gotoPrev();
  bool8 gotoMark();

  // method: gotoPosition
  //
  bool8 gotoPosition(int32 number) {
    NODE* node = getNode(number);
    if (node == (NODE*)NULL) {
      return false;
    }
    curr_d = node;
    return true;
  }

  // method to find the index of the current element
  //
  int32 getPosition() const;

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  marking methods
  //
  //---------------------------------------------------------------------------
  
  // method: markIsSet
  //
  bool8 markIsSet() const {
    return (mark_d != (NODE*)NULL);
  }

  // method: isMarkedElement
  //
  bool8 isMarkedElement() const {
    return (markIsSet() && (curr_d == mark_d));
  }

  // method: clearMark
  //
  bool8 clearMark() {
    mark_d = (NODE*)NULL;
    return true;
  }

  // other marked node methods:
  //  methods to check, set and clear the marked node
  //  
  bool8 setMark();
  
  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  access methods
  //
  //---------------------------------------------------------------------------
  
  // method: getFirst
  //
  TObject* getFirst() {
    if (first_d != (NODE*)NULL) {
      return first_d->getItem();
    }
    return (TObject*)NULL;
  }
  
  // method: getFirst
  //
  const TObject* getFirst() const {
    if (first_d != (NODE*)NULL) {
      return first_d->getItem();
    }
    return (TObject*)NULL;
  }

  // method: getLast
  //
  TObject* getLast() {
    if (last_d != (NODE*)NULL) {
      return last_d->getItem();
    }
    return (TObject*)NULL;
  }

  // method: getLast
  //
  const TObject* getLast() const {
    if (last_d != (NODE*)NULL) {
      return last_d->getItem();
    }
    return (TObject*)NULL;
  }
  
  // method: getNext
  //
  TObject* getNext() {
    if ((curr_d != (NODE*)NULL)
	&& (curr_d->getNext() != (NODE*)NULL)) {
      return curr_d->getNext()->getItem();
    }
    return (TObject*)NULL;
  }

  // method: getNext
  //
  const TObject* getNext() const {
    if ((curr_d != (NODE*)NULL)
	&& (curr_d->getNext() != (NODE*)NULL)) {
      return curr_d->getNext()->getItem();
    }
    return (TObject*)NULL;
  }
  
  // method: getPrev
  //
  TObject* getPrev() {
    if ((curr_d != (NODE*)NULL)
	&& (curr_d->getPrev() != (NODE*)NULL)) {
      return curr_d->getPrev()->getItem();
    }
    return (TObject*)NULL;
  }

  // method: getPrev
  //
  const TObject* getPrev() const {
    if ((curr_d != (NODE*)NULL)
	&& (curr_d->getPrev() != (NODE*)NULL)) {
      return curr_d->getPrev()->getItem();
    }
    return (TObject*)NULL;
  }
  
  // method: getMark
  //
  TObject* getMark() {
    if (mark_d != (NODE*)NULL) {
      return mark_d->getItem();
    }
    return (TObject*)NULL;
  }
  
  // method: getMark
  //
  const TObject* getMark() const {
    if (mark_d != (NODE*)NULL) {
      return mark_d->getItem();
    }
    return (TObject*)NULL;
  }

  // method: getCurr
  //
  TObject* getCurr() {
    if (curr_d != (NODE*)NULL) {
      return curr_d->getItem();
    }
    return (TObject*)NULL;
  }

  // method: getCurr
  //
  const TObject* getCurr() const {
    if (curr_d != (NODE*)NULL) {
      return curr_d->getItem();
    }
    return (TObject*)NULL;
  }

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  insert and remove methods
  //
  //---------------------------------------------------------------------------
  
  // insert and remove methods:
  //  the current pointer is moved to the node inserted. 
  //
  bool8 insert(TObject* item);
  bool8 insert(DoubleLinkedList<TObject>& ilist);
  bool8 remove(TObject*& item);
  bool8 remove();
  
  bool8 insertFirst(TObject* item);
  bool8 insertFirst(DoubleLinkedList<TObject>& ilist);    
  bool8 removeFirst(TObject*& item);
  bool8 removeFirst();
  
  bool8 insertLast(TObject* item);
  bool8 insertLast(DoubleLinkedList<TObject>& ilist);    
  bool8 removeLast(TObject*& item);
  bool8 removeLast();

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  property methods
  //
  //---------------------------------------------------------------------------
  
  // method: isEmpty
  //
  bool8 isEmpty() const {
    return ((int32)length_d == 0);
  }

  // method: isFirst
  //
  bool8 isFirst() const {
    return (!isEmpty() && (curr_d == first_d));
  }

  // method: isLast
  //
  bool8 isLast() const {
    return (!isEmpty() && (curr_d == last_d));
  }

  // method: length
  //
  int32 length() const {
    return length_d;
  }
  
  // item containment methods:
  //  methods to determine if the specified object is contained in the
  //  list. the find method leaves the list positioned at the first
  //  found element, the contains method leaves the list unchanged.
  //
  bool8 find(const TObject* item);
  bool8 contains(const TObject* item) const;

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  ordering methods
  //
  //---------------------------------------------------------------------------  
  // sort and reverse methods:
  //
  bool8 sort(Integral::ORDER sort_order = Integral::ASCENDING,
	       SORT_ALGO = DEF_SORT_ALGO);  
  bool8 reverse();

  // swap methods:
  //  swap two nodes in the list given the node indices
  //  
  bool8 swap(int32 i, int32 j);

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  apply methods
  //
  //---------------------------------------------------------------------------
  
  // apply methods:
  //  apply an external function to each element in the list methods
  //
  bool8 apply(bool8 (TObject::*method)());
  bool8 apply(bool8 (TObject::*method)(), DoubleLinkedList<TObject>& arg);

  //---------------------------------------------------------------------------
  //
  // class-specific public methods:
  //  allocation mode methods
  //
  //---------------------------------------------------------------------------
  
  // method: getAllocationMode
  //
  ALLOCATION getAllocationMode() const {
    return alloc_d;
  }

  // method: setAllocationMode
  //
  bool8 setAllocationMode(ALLOCATION alloc) {
    alloc_d = alloc;
    return true;
  }

  //---------------------------------------------------------------------------
  //
  // private methods
  //
  //---------------------------------------------------------------------------
private:

  // sort methods
  //
  bool8 randQuickSort(Integral::ORDER sort_order);
  bool8 insertionSort(Integral::ORDER sort_order);

  // swap methods:
  //  exchange two nodes in the list, this method assume the i_node
  //  and j_node are in the list and i_node is in front of j_node
  //  the avoids the overhead of checking and is used inside sort
  //
  bool8 swap(NODE*& i_node, NODE*& j_node);
  
  // node addressing methods
  //
  NODE* getNode(int32 index) const;

  // friend class
  //
  template <class TObject_diagnose> 
  friend class DoubleLinkedListDiagnose;  
};

//-----------------------------------------------------------------------------
//
// we define non-integral constants at the end of class definition for
// templates (for non-templates these are defined in the default constructor)
//      
//-----------------------------------------------------------------------------

// constants: required constants such as the class name
//
template <class TObject>
const String DoubleLinkedList<TObject>::CLASS_NAME(L"DoubleLinkedList");

template <class TObject>
const String DoubleLinkedList<TObject>::DEF_PARAM(L"values");

// static instantiations: debug level and memory manager
//
template <class TObject>
Integral::DEBUG DoubleLinkedList<TObject>::debug_level_d = Integral::NONE;

template <class TObject>
MemoryManager DoubleLinkedList<TObject>::mgr_d(sizeof(DoubleLinkedList<TObject>), CLASS_NAME);

// below are all the methods for the DoubleLinkedList template class
//

//----------------------------------------------------------------------
//
// required static methods
//
//----------------------------------------------------------------------

// method: name
//
// arguments: none
//
// return: a static String& containing the class name
//
// this method returns the class name
//
template<class TObject>
const String& DoubleLinkedList<TObject>::name() {
  
  // create the static name string for this class and return it
  //
  static String cname(CLASS_NAME);
  cname.clear();
  cname.concat(CLASS_NAME);
  cname.concat(L"<");
  cname.concat(TObject::name());
  cname.concat(L">");
  
  // return the name
  //
  return cname;
}

//----------------------------------------------------------------------
//
// required debug methods
//
//----------------------------------------------------------------------

// method: debug
//
// arguments:
//  const unichar* message: (input) information message
//
// return: a bool8 value indicating status
//
// this method dumps the contents of an object to the console
// 
template<class TObject>
bool8 DoubleLinkedList<TObject>::debug(const unichar* message_a) const {

  // declare local variables
  //
  String output;
  String value;
  String extras;
  
  // print out the length
  //
  value.assign((int32)length_d);
  output.debugStr(name(), message_a, L"length_d", value);
  Console::put(output);
  
  // print out the memory allocation mode
  //
  output.debugStr(name(), message_a, L"alloc_d",
		  NameMap::ALLOCATION_MAP((int32)alloc_d));
  Console::put(output);

  // print out the debug level
  //
  value.assign(debug_level_d);
  output.debugStr(name(), message_a, L"debug_level_d", value);
  Console::put(output);
  
  NODE* temp_curr = curr_d;
  
  // loop over each node and print its value
  //
  const_cast< DoubleLinkedList<TObject>* >(this)->gotoFirst();
  
  for (int32 i = 0; i < (int32)length_d; i++) {

    extras.clear();
    
    if (curr_d == first_d) {
      extras.concat(L", ");
      extras.concat(L"first");
    }
    if (curr_d == last_d) {
      extras.concat(L", ");
      extras.concat(L"last");
    }
    if (curr_d == mark_d) {
      extras.concat(L", ");
      extras.concat(L"mark");
    }
    if (curr_d == temp_curr) {
      extras.concat(L", ");
      extras.concat(L"current");
    }
    
    // trim extra , character and clear the string
    //
    extras.trimLeft(L", ");
    output.clear();
    
    if (extras.length() > 0) {
      output.debugStr(name(), message_a, L"node", extras);
    }
    else {
      output.debugStr(name(), message_a, L"node");
    }
    
    Console::put(output);
    
    // call the debug method of the element
    //
    Console::increaseIndention();
    curr_d->debug(message_a);
    Console::decreaseIndention();
    
    // go to next node
    //
    const_cast< DoubleLinkedList<TObject>* >(this)->gotoNext();
  }  
  
  // restore the current node
  //
  const_cast< DoubleLinkedList<TObject>* >(this)->curr_d = temp_curr;
  
  // exit gracefully
  // 
  return true;
}

//------------------------------------------------------------------------
//
// required destructor/constructor(s)
//
//-----------------------------------------------------------------------

// method: default constructor
//
// arguments:
//  ALLOCATION alloc: (input) allocation mode for the list
//
// return: none
//
template<class TObject>
DoubleLinkedList<TObject>::DoubleLinkedList(ALLOCATION alloc_a) {
  
  // initialize the pointers to the first, last, current and the marked nodes
  //
  first_d = (NODE*)NULL;
  last_d = (NODE*)NULL;
  curr_d = (NODE*)NULL;
  mark_d = (NODE*)NULL;

  // initialize memory allocation flag
  //
  alloc_d = alloc_a;
  
  // initialize the length
  //
  length_d = (int32)0;  

  // exit gracefully
  //
}

// method: copy constructor
//
// arguments:
//  DoubleLinkedList<TObject>& copy_list: (input) the list to copy
//
// return: none
//
template<class TObject>
DoubleLinkedList<TObject>::DoubleLinkedList(const DoubleLinkedList<TObject>&
					    copy_list_a) {
  
  // initialize the pointers to the first, last, current and the marked nodes
  //
  first_d = (NODE*)NULL;
  last_d = (NODE*)NULL;
  curr_d = (NODE*)NULL;
  mark_d = (NODE*)NULL;

  // copy the memory allocation flag
  //
  alloc_d = copy_list_a.alloc_d;
  
  // initialize the length
  //
  length_d = (int32)0;
  
  // call the assign method to copy the list
  //
  assign(copy_list_a);
}

//------------------------------------------------------------------------
//
// required assign methods
//
//-------------------------------------------------------------------------

// method: assign
//
// arguments:
//  const DoubleLinkedList<TObject>& copy_list: (input) the list to copy
//
// return: a bool8 value indicating status
//
// this method copies the contents of the input to this list
//
template<class TObject>
bool8
DoubleLinkedList<TObject>::assign(const DoubleLinkedList<TObject>&
				  copy_list_a) {
  
  // clear this list
  //
  clear();
  
  // store the state of the copy list
  //
  NODE* tmp_node = copy_list_a.first_d;

  // create a variable to store the current node pointer
  //
  NODE* tmp_curr = (NODE*)NULL;
  
  // loop over each element and insert each element onto the new
  // list. note that we don't use the standard gotoFirst, gotoNext
  // methods so that we can easily copy over the exact state of the
  // given list (including the current pointer)
  //
  while (tmp_node != (NODE*)NULL) {
    
    // add the next element to the list
    //
    insert(tmp_node->getItem());

    // see if this is the marked element
    //
    if (tmp_node == copy_list_a.mark_d) {

      // set the marked element for this list
      //
      setMark();
    }

    // see if this is the current element
    //
    if (tmp_node == copy_list_a.curr_d) {

      // set the marked element for this list
      //
      tmp_curr = curr_d;
    }

    // move to the next node
    //
    tmp_node = tmp_node->getNext();
  }

  // set curr_d
  //
  curr_d = tmp_curr;
  
  // the length should be set by the inserts
  // 
  if (length_d != copy_list_a.length_d) {
    return Error::handle(name(), L"assign", ERR, __FILE__, __LINE__);
  }
  
  // exit gracefully
  //
  return true;
}

//------------------------------------------------------------------------
//
// required i/o methods
//
//------------------------------------------------------------------------

// method: sofSize
//
// arguments: none
//
// return: size of object as written to disk via the i/o methods
//
// this method determines the size of the object on disk
//
template<class TObject>
int32 DoubleLinkedList<TObject>::sofSize() const {
  
  // declare temporary variables
  //
  NODE* tmp_node = first_d;
  int32 tmp_size = 0;
  
  // count the size of length_d
  //
  tmp_size = length_d.sofSize();
  
  // loop over each node and add the size of that node. this is the
  // total size of the list
  //
  while (tmp_node != (NODE*)NULL) {
    
    // add the size of this node
    //
    tmp_size += tmp_node->sofSize();
    
    // move to the next node
    //
    tmp_node = tmp_node->getNext();
  }
  
  // return the size
  //
  return tmp_size;
}

// method: read
//
// arguments:
//  Sof& sof: (input) sof file object
//  int32 tag: (input) sof object instance tag
//  const String& name: (input) sof object instance name
//
// return: a bool8 value indicating status
//
// this method has the object read itself from an Sof file
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::read(Sof& sof_a, int32 tag_a,
					const String& name_a) {

  // get the instance of the object from the Sof file
  //
  if (!sof_a.find(name_a, tag_a)) {
    return false;
  }
  
  // read the actual data from the sof file
  //
  if (!readData(sof_a)) {
    return false;
  }
  
  // exit gracefully
  //
  return true;
}

// method: write
//
// arguments:
//  Sof& sof: (input) sof file object
//  int32 tag: (input) sof object instance tag
//  const String& name: (input) sof object instance name
//
// return: a bool8 value indicating status
//
// this method has the object write itself to an Sof file
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::write(Sof& sof_a, int32 tag_a,
					 const String& name_a) const {
  
  // declare a temporary size variable
  //
  int32 obj_size = 0;
  
  // switch on ascii or binary mode
  //
  if (sof_a.isText()) {
    
    // set the size to be dynamic
    //
    obj_size = Sof::ANY_SIZE;
  }
  else {
    
    // the size of the binary data to write
    //
    obj_size = sofSize();
  }

  // put the object into the sof file's index
  //
  if (!sof_a.put(name_a, tag_a, obj_size)) {
    return false;
  }
  
  // exit gracefully
  //
  return writeData(sof_a);
}

// method: readData
//
// arguments:
//  Sof& sof: (input) sof file object
//  const String& pname: (input) parameter name
//  int32 size: (input) size of the object
//  bool8 param: (input) is the parameter specified?
//  bool8 nested: (input) is this nested?
//
// return: a bool8 value indicating status
//
// this method has the object read itself from an Sof file. it assumes
// that the Sof file is already positioned correctly.
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::readData(Sof& sof_a, const String& pname_a,
					    int32 size_a, bool8 param_a,
					    bool8 nested_a) {
  
  // first clear the list
  //
  if (!clear()) {
    return Error::handle(name(), L"readData", Error::READ,
			 __FILE__, __LINE__, Error::WARNING);
  }
  
  // local variables
  //
  SofParser parser;
  String pname;

  // set the parser debug level
  //
  parser.setDebug(debug_level_d);
  
  // if param is false, this means implicit parameter
  //
  if (!param_a) {
    parser.setImplicitParam();
    pname.assign(parser.implicitPname());
  }
  else {
    pname.assign(pname_a);
  }
  
  // are we nested?
  //
  if (nested_a) {
    parser.setNest();
  }
  
  // load the parse
  //
  if (!parser.load(sof_a, size_a)) {
    return Error::handle(name(), L"readData", Error::READ,
			 __FILE__, __LINE__, Error::WARNING);
  }
  
  Long new_size((int32)0);
  
  // read the length first: this differs for text or binary
  //
  if (sof_a.isText()) {
    new_size = parser.countTokens(pname);
  }
  
  // binary mode
  //
  else {
    if (!new_size.readData(sof_a, pname)) {
      return false;
    }
  }

  // change to USER allocation mode, but save the old state
  //
  ALLOCATION alloc = alloc_d;
  alloc_d = USER;

  for (int32 i = 0; i < new_size; i++) {
    
    // we need to new an element
    //
    TObject* new_obj = new TObject();
    
    // read the node
    //
    if (!new_obj->readData(sof_a, pname, parser.getEntry(sof_a, pname, i, 1),
			   false, true)) {
      alloc_d = alloc;
      return Error::handle(name(), L"readData", Error::READ,
			   __FILE__, __LINE__, Error::WARNING);
    }
    
    // insert the node
    //
    if (!insert(new_obj)) {
      alloc_d = alloc;
      return Error::handle(name(), L"readData", Error::READ,
			   __FILE__, __LINE__, Error::WARNING);
    }
  }

  // restore the allocation mode
  //
  alloc_d = alloc;
  
  // exit gracefully
  //
  return true;
}

// method: writeData
//
// arguments:
//  Sof& sof: (input) sof file object
//  const String& pname: (input) parameter name
//
// return: a bool8 value indicating status
//
// this method writes the object to the Sof file. it assumes that the
// Sof file is already positioned correctly.
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::writeData(Sof& sof_a, const String&
					     pname_a) const {
  
  // we need an empty string for the sub-parameter
  //
  String empty_str;
  String start_str;
  start_str.assign(SofParser::DEF_BLOCKSTART_CHAR);       // L"{"
  start_str.concat(SofParser::NEWLINE_CHAR);
  
  String end_str(SofParser::NEWLINE_CHAR);
  end_str.assign(SofParser::DEF_BLOCKSTOP_CHAR);          // L"}"
  String delim_str(end_str);
  delim_str.concat(SofParser::DEF_DELIMITER_CHAR);       
  delim_str.concat(SofParser::SPACE_CHAR);
  delim_str.concat(start_str);                           // L"}, {"
  
  String term;
  term.assign(SofParser::DEF_TERMINATOR_CHAR);
  term.concat(SofParser::NEWLINE_CHAR);                  // L";\n"
    
  // if text, write a parameter name
  //
  if (sof_a.isText()) {
    
    if (pname_a.length() > 0) {
      String output;
      output.assign(pname_a);
      output.concat(SofParser::SPACE_CHAR);
      output.concat(SofParser::DEF_ASSIGNMENT_CHAR);
      output.concat(SofParser::SPACE_CHAR);
      sof_a.puts(output);
    }

    if ((int32)length_d > 0) {
      sof_a.puts(start_str);
    }
  }
  
  // for binary, write length
  //
  else {
    
    if (!length_d.writeData(sof_a)) {
      return Error::handle(name(), L"writeData", Error::IO,__FILE__,__LINE__);
    }
  }
  
  bool8 is_first = true;

  // save the state of the list
  //
  NODE* tmp_node = first_d;
  
  // loop through the list
  //
  while (tmp_node != (NODE*)NULL) {
    
    if (sof_a.isText()) {
      if (!is_first) {
	sof_a.decreaseIndention();
	sof_a.puts(delim_str);
      }
      sof_a.increaseIndention();
      is_first = false;
    }
    
    // write this element
    //
    TObject* ptr = tmp_node->getItem();
    if (ptr != (TObject*)NULL) {
      if (!ptr->writeData(sof_a, empty_str)) {
	return Error::handle(name(), L"writeData",Error::IO,__FILE__,__LINE__);
      }
    }
    else {
      return Error::handle(name(), L"writeData", Node<TObject>::ERR_NOITEM,
			   __FILE__, __LINE__);
    }

    // move to the next node
    //
    tmp_node = tmp_node->getNext();
  }
  
  if (sof_a.isText()) {
    
    // write the close brace
    //
    if ((int32)length_d > 0) {
      sof_a.decreaseIndention();
      sof_a.puts(end_str);
    }
    
    // possibly terminate the statement
    //
    if (pname_a.length() > 0) {
      sof_a.puts(term);
    }
  }

  // exit gracefully
  //
  return true;
}

// ----------------------------------------------------------------------
//
// required equality methods
//
//------------------------------------------------------------------------

// method: eq
//
// arguments:
//  const DoubleLinkedList<TObject>& compare_list: (input) the list to compare
//
// return: bool8 value indicating test of equivalence
//
// this method compares two lists for equivalence. two lists are equivalent
// if all corresponding nodes are equivalent
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::eq(const DoubleLinkedList<TObject>&
				      compare_list_a) const {
  
  // declare the output variable
  //
  bool8 are_equal = true;
  
  // declare temporary variables to index into the lists
  //
  NODE* node_ptr_1 = first_d;
  NODE* node_ptr_2 = compare_list_a.first_d;

  // two lists can not be equivalent if they are of different lengths
  //
  if (length_d != compare_list_a.length_d) {
    
    // set the break flag
    //
    are_equal = false;
  }
  
  // loop over each node and see if each is equivalent
  //
  while ((are_equal) &&
	 (node_ptr_1 != (NODE*)NULL) &&
	 (node_ptr_2 != (NODE*)NULL)) {

    // see if the current nodes are equal
    //
    are_equal = node_ptr_1->eq(*node_ptr_2);
    
    if (are_equal) {
      
      // get the next nodes
      //
      node_ptr_1 = node_ptr_1->getNext();
      node_ptr_2 = node_ptr_2->getNext();
    }
  }
  
  // return a value representing if they are equivalent
  //
  return are_equal;
}

//-------------------------------------------------------------------------
//
// required memory management methods
//
//-------------------------------------------------------------------------

// method: clear
//
// arguments:
//  Integral::CMODE cmode_a: (input) clear mode
//  
// return: a bool8 value indicating status
//
// this method clears the contents of the list by the setting of cmode_a
//
//  enum CMODE { RETAIN = 0, RESET, RELEASE, FREE, DEF_CMODE = RESET };
//
// RETAIN: call clear with RETAIN on each element in the list
//
// RESET: delete items from the front of the list until the list is empty
//
// RELEASE: delete items from the front of the list until the list is empty
//
// FREE: remove all nodes from the list and clear the corresponding nodes
//
// Programming hint:
//
// use the clear() method to manage the memory of the objects going
// into the list.
// particular useful when list is in USER mode. Caution the object
// you place into list must have a clear method that meets the
// requirements of the IFC clear method.
//
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::clear(Integral::CMODE cmode_a) {
  
  // temporary variables
  //
  TObject* tmp_item;

  // if cmode_a is called with RETAIN then call clear with RETAIN on
  // each element in the list
  //
  if (cmode_a == Integral::RETAIN) {
    for (bool8 more = gotoFirst(); more; more = gotoNext()) {    
      curr_d->getItem()->clear(Integral::RETAIN);
    } 
  }

  // if cmode_a is called with RESET or RELEASE remove all nodes form
  // the list
  //
  if ((cmode_a == Integral::RESET) || (cmode_a == Integral::RELEASE)) {

    // delete items from the front of the list until the list is empty
    //
    while (removeFirst());
  }

  // if cmode_a is called with FREE remove all nodes form the list and
  // clear the corresponding nodes
  //  
  if (cmode_a == Integral::FREE) {

    if (alloc_d == SYSTEM) {
      for (gotoFirst(); !isEmpty(); remove()) {
	curr_d->clear(cmode_a);
      }
    }
    else { 
      while (removeFirst(tmp_item)) {
	tmp_item->clear(cmode_a);
	delete tmp_item;
      }
    }    
  }
  
  // exit gracefully
  //
  return true;
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  overload operators and extensions to required methods
//
//---------------------------------------------------------------------------

// method: operator ()
//
// arguments:
//  int32 index: (input) the index with respect to the first_d
//
// return: TObject& of the given index
//
// this method gets the item of the given index in the list
//
template<class TObject>
TObject& DoubleLinkedList<TObject>::operator() (int32 index_a) {
  
  // find the node
  //
  NODE* pp = getNode(index_a);

  if (pp == (NODE*)NULL) {
    Error::handle(name(), L"operator ()", ERR, __FILE__, __LINE__);
    static TObject tmp;
    return tmp;    
  }
      
  // return the item inside the node
  //
  return *(pp->getItem());
  
}

// method: operator ()
//
// arguments:
//  int32 index: (input) the index with respect to the first_d
//
// return: TObject& of the given index
//
// this method gets the item of the given index in the list
//
template<class TObject>
const TObject& DoubleLinkedList<TObject>::operator() (int32 index_a) const {
  
  // find the node
  //
  NODE* pp = getNode(index_a);

  if (pp == (NODE*)NULL) {
    Error::handle(name(), L"operator ()", ERR, __FILE__, __LINE__);
    static TObject tmp;
    return tmp;    
  }
      
  // return the item inside the node
  //
  return *(pp->getItem());
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  positioning methods
//
//---------------------------------------------------------------------------

// method: gotoNext
//
// arguments: none
//
// return: a bool8 value indicating status
//
// this method makes the current node pointer jump to the next node
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::gotoNext() {

  // determine if the current and next nodes are valid
  //
  if ((curr_d == (NODE*)NULL) ||
      (curr_d->getNext() == (NODE*)NULL)) {
    return false;
  }

  // make the current node jump to the next node
  //
  curr_d = curr_d->getNext();

  // exit gracefully
  //
  return true;
}

// method: gotoPrev
//
// arguments: none
//
// return: a bool8 value indicating status
//
// this method makes the current node pointer jump to the previous node
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::gotoPrev() {
  
  // determine if the current node is valid
  //
  if ((curr_d == (NODE*)NULL) ||
      (curr_d->getPrev() == (NODE*)NULL)) {
    return false;
  }
  
  // make the current node jump to the previous node
  //
  curr_d = curr_d->getPrev();

  // exit gracefully
  //
  return true;
}

// method: gotoMark
//
// arguments: none
//
// return: a bool8 value indicating status
//
// this method makes the current node pointer jump to the marked node
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::gotoMark() {

  // make sure that the marked location is valid
  //
  if (mark_d == (NODE*)NULL) {
    return false;
  }

  // set the node back to mark
  //
  curr_d = mark_d;

  // exit gracefully
  //
  return true;
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  marking methods
//
//---------------------------------------------------------------------------
  
// method: setMark
//
// arguments: none
//  
// return: a bool8 value indicating status
//
// this method sets the current node to be the marked node
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::setMark() {
  
  // check if the current node is NULL 
  //
  if (curr_d == (NODE*)NULL) {
    return false;
  }
    
  // mark the current node 
  //
  mark_d = curr_d;
  
  // exit gracefully
  //
  return true;
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  insert and remove methods
//
//---------------------------------------------------------------------------

// method: insert
//
// arguments:
//  TObject* item: (input) the item to be added
//  
// return: a bool8 value indicating status
//
// this method adds a new node containing the item after the current position
// of this list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::insert(TObject* item_a) {
  
  // make sure the item is not NULL
  //
  if (item_a == (TObject*)NULL) {
    return Error::handle(name(), L"insert", Error::NULL_ARG,
			 __FILE__, __LINE__);
  }

  // the list was empty, add the input item to the beginning
  //
  if (isEmpty()) {
    return insertFirst(item_a);
  }
  
  // if curr_d is the last node in the old list then add the new item
  // at the end of the list
  //
  else if (curr_d == last_d) {
    return insertLast(item_a);
  }

  // if the list is not empty then add it as the next item in the list
  //
  
  // create a new node and put or copy the item into it
  //
  NODE* new_node;
  
  // SYSTEM mode will store a copy, USER mode stores just the reference
  //
  if (alloc_d == SYSTEM) {
    new_node = new NODE(new TObject(*item_a));
  }
  else {
    new_node = new NODE(item_a);
  }
  
  // set the links for the new node
  //
  new_node->setNext(curr_d->getNext());
  new_node->setPrev(curr_d);
  
  // set the links for the surrounding nodes
  //
  curr_d->setNext(new_node);
  
  if (new_node->getNext() != (NODE*)NULL) {
    new_node->getNext()->setPrev(new_node);
  }
  
  // move the current node pointer
  //
  curr_d = new_node;
  
  // increment the size of the list
  //
  length_d += 1;
  
  // exit gracefully
  //
  return true;
}

// method: insert
//
// arguments:
//  DoubleLinkedList<TObject>& ilist: (input) the list to be added into
//                                    this list
//  
// return: a bool8 value indicating status
//
// this method adds a list into the current position of this list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::insert(DoubleLinkedList<TObject>&
					  ilist_a) {
  
  // if the input list is empty then there is nothing to do
  //
  if (ilist_a.isEmpty()) {
    
    // exit gracefully
    //
    return true;
  }
  
  // store the state of the input list
  //
  NODE* tmp_node = ilist_a.first_d;
  
  // loop over each element in the input list and add each element to the list
  //
  while (tmp_node != (NODE*)NULL) {
    
    // add the next element to the list
    //
    insert(tmp_node->getItem());

    // move to the next node
    //
    tmp_node = tmp_node->getNext();
  }
  
  // exit gracefully
  //
  return true;
}

// method: remove
//
// arguments:
//  TObject*& item: (output) the item in the current node
//  
// return: a bool8 value indicating status
//
// this method removes the current node and outputs the item inside
// it. if the allocation mode is USER then the pointer returned will
// be the original item. if in SYSTEM, the user must pass in an
// allocated object and we will copy the internal data to this
// pointer, since we have to delete the internal pointer.
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::remove(TObject*& item_a) {

  // check curr_d
  //
  if (curr_d == (NODE*)NULL) {
    return false;
  }
  
  // make sure memory is allocated if we are SYSTEM-allocated
  //
  if ((alloc_d == SYSTEM) && (item_a == (TObject*)NULL)) {
    return (Error::handle(name(), L"remove", Error::NULL_ARG,
			  __FILE__, __LINE__));
  }

  // else initialize the output argument
  //
  else if (alloc_d == USER) {
    item_a = (TObject*)NULL;
  }

  // unlink the current node and handle special cases
  //
  // if the marked node just got deleted then set it to NULL
  //
  if (curr_d == mark_d) {
    mark_d = (NODE*)NULL;
  }  
  if (curr_d == first_d) {
    return removeFirst(item_a);
  }
  else if (curr_d == last_d) {
    return removeLast(item_a);
  }
  
  // this will either save the pointer (for USER mode) or make a copy
  // (for SYSTEM mode).
  //
  item_a = curr_d->accessByMode(item_a, alloc_d);
  
  // if allocation mode is SYSTEM, delete the pointer
  //
  if (alloc_d == SYSTEM) {
    delete curr_d->getItem();
  }
  
  // handle the general case. if curr_d is not the first or the last
  // then its link pointers can not be NULL
  //
  curr_d->getPrev()->setNext(curr_d->getNext());
  curr_d->getNext()->setPrev(curr_d->getPrev());
  
  // decrement the length of the list
  //
  length_d -= 1;

  // delete the node and set the current pointer to the next node
  //
  NODE* ptr = curr_d->getNext();
  delete curr_d;
  curr_d = ptr;
  
  // exit gracefully
  //
  return true;
}

// method: remove
//
// arguments: none
//  
// return: a bool8 value indicating status
//
// this method removes the current node
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::remove() {
  
  // check curr_d
  //
  if (curr_d == (NODE*)NULL) {
    return false;
  }
  
  // if the marked node just got deleted then set it to NULL
  //
  if (curr_d == mark_d) {
    mark_d = (NODE*)NULL;
  }  
  if (curr_d == first_d) {
    return removeFirst();
  }
  else if (curr_d == last_d) {
    return removeLast();
  }
  
  // if allocation mode is SYSTEM, delete the pointer
  //
  if (alloc_d == SYSTEM) {
    delete curr_d->getItem();
  }
  
  // handle the general case. if curr_d is not the first or the last
  // then its link pointers can not be NULL
  //
  curr_d->getPrev()->setNext(curr_d->getNext());
  curr_d->getNext()->setPrev(curr_d->getPrev());
  
  // decrement the length of the list
  //
  length_d -= 1;

  // delete the node and set the current pointer to the next node
  //
  NODE* ptr = curr_d->getNext();
  delete curr_d;
  curr_d = ptr;
  
  // exit gracefully
  //
  return true;
}

// method: insertFirst
//
// arguments:
//  TObject* item: (input) the item to be added into the new first node
//  
// return: a bool8 value indicating status
//
// this method adds a new node containing the item into the beginning
// of this list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::insertFirst(TObject* item_a) {
  
  // make sure the item is not NULL
  //
  if (item_a == (TObject*)NULL) {
    return Error::handle(name(), L"insertFirst", Error::NULL_ARG, __FILE__,
                         __LINE__);
  }
  
  // create a new node and put or copy the item into it
  //
  NODE* new_node;

  // SYSTEM mode will store a copy, USER mode stores just the reference
  //
  if (alloc_d == SYSTEM) {
    new_node = new NODE(new TObject(*item_a));
  }
  else {
    new_node = new NODE(item_a);
  }
  
  // set the links for the new node
  //
  new_node->setNext(first_d);
  new_node->setPrev((NODE*)NULL);
  
  // set the links for surrounding nodes
  //
  if (first_d != (NODE*)NULL) {
    new_node->getNext()->setPrev(new_node);
  }
  
  // if the list was previously empty then set first, last, and current
  // to the new node. otherwise, just set the first node to the new node
  //
  if (isEmpty()) {
    last_d = new_node;
  }
  first_d = new_node;
  curr_d = first_d;
  
  // increment the size of the list
  //
  length_d += 1;
  
  // exit gracefully
  //
  return true;
}

// method: insertFirst
//
// arguments:
//  DoubleLinkedList<TObject>& ilist: (input) the list to be added into
//                                    this list
//  
// return: a bool8 value indicating status
//
// this method adds a list into the beginning of this list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::insertFirst(DoubleLinkedList<TObject>&
					       ilist_a) {
  
  // if the input list is empty then there is nothing to do
  //
  if (ilist_a.isEmpty()) {
    
    // exit gracefully
    //
    return true;
  }
  
  // start at the back of the input list
  //
  NODE* tmp_node = ilist_a.last_d;
  
  // loop over each element in the input list and add each element to the list
  //
  while (tmp_node != (NODE*)NULL) {
    
    // add the next element to the list
    //
    insertFirst(tmp_node->getItem());

    // move to the previous node
    //
    tmp_node = tmp_node->getPrev();
  }
  
  // exit gracefully
  //
  return true;
}

// method: removeFirst
//
// arguments:
//  TObject*& item: (output) the item in the first node
//  
// return: a bool8 value indicating status
//
// this method removes the first node and outputs the item inside it.
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::removeFirst(TObject*& item_a) {
  
  // make sure the first node is valid
  //
  if (first_d == (NODE*)NULL) {
    return false;
  }
  
  // make sure the memory is allocated if in SYSTEM mode
  //
  if ((alloc_d == SYSTEM) && (item_a == (TObject*)NULL)) {
    return (Error::handle(name(), L"removeFirst", Error::NULL_ARG,
			  __FILE__, __LINE__));
  }

  // else initialize the output argument
  //
  else if (alloc_d == USER) {
    item_a = (TObject*)NULL;
  }
  
  // if allocation mode is USER, just return the pointer
  //
  if (alloc_d == USER) {
    item_a = first_d->getItem();
  }
  
  // for system mode assign to the output pointer and delete the item
  // memory
  //
  else {
    item_a->assign(*(first_d->getItem()));
    delete first_d->getItem();
  }
  
  // keep the position of the first node
  //
  NODE* tmp_node = first_d;
  
  // unlink the first node and set the new first node
  //  if the first node is not equal to the last node then there must be
  //  nodes in between. otherwise, the new list will be empty
  //
  if (first_d != last_d) {
    first_d->getNext()->setPrev((NODE*)NULL);
    first_d = first_d->getNext();
  }
  else {
    first_d = (NODE*)NULL;
    last_d = (NODE*)NULL;
    curr_d = (NODE*)NULL;
  }
  
  // if the current node just got deleted then set the new current to be
  // the new first node
  //
  if (curr_d == tmp_node) {
    curr_d = first_d;
  }
  
  // if the marked node just got deleted then set it to NULL
  //
  if (mark_d == tmp_node) {
    mark_d = (NODE*)NULL;
  }

  // delete the old node and decrement the length of the list
  //
  delete tmp_node;
  length_d -= 1;
  
  // exit gracefully
  //
  return true;
}

// method: removeFirst
//
// arguments: none
//  
// return: a bool8 value indicating status
//
// this method removes the first node
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::removeFirst() {
  
  // make sure the first node is valid
  //
  if (first_d == (NODE*)NULL) {
    
    // return an error
    //
    return false;
  }
  
  // keep the position of the first node
  //
  NODE* tmp_node = first_d;
  
  // unlink the first node and set the new first node
  //  if the first node is not equal to the last node then there must be
  //  nodes in between. otherwise, the new list will be empty
  //
  if (first_d != last_d) {
    first_d->getNext()->setPrev((NODE*)NULL);
    first_d = first_d->getNext();
  }
  else {
    first_d = (NODE*)NULL;
    last_d = (NODE*)NULL;
    curr_d = (NODE*)NULL;
  }
  
  // if the current node just got deleted then set the new current to be
  // the new first node
  //
  if (curr_d == tmp_node) {
    curr_d = first_d;
  }
  
  // if the marked node just got deleted then set it to NULL
  //
  if (mark_d == tmp_node) {
    mark_d = (NODE*)NULL;
  }
  
  // delete item memory if we are SYSTEM-allocated
  //
  if (alloc_d == SYSTEM) {
    delete tmp_node->getItem();
  }
  
  // delete the old node and decrement the length of the list
  //
  delete tmp_node;
  length_d -= 1;
  
  // exit gracefully
  //
  return true;
}

// method: insertLast
//
// arguments:
//  TObject* item: (input) the item to be added into the last node
//  
// return: a bool8 value indicating status
//
// this method adds a new node containing the item into the end of this list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::insertLast(TObject* item_a) {
  
  // make sure the item is not NULL
  //
  if (item_a == (TObject*)NULL) {
    return Error::handle(name(), L"insertLast", Error::NULL_ARG, __FILE__,
                         __LINE__);
  }
  
  // create a new node and put or copy the item into it
  //
  NODE* new_node;

  // SYSTEM mode will store a copy, USER mode stores just the reference
  //
  if (alloc_d == SYSTEM) {
    new_node = new NODE(new TObject(*item_a));
  }
  else {
    new_node = new NODE(item_a);
  }
  
  // set the links for the new node
  //
  new_node->setNext((NODE*)NULL);
  new_node->setPrev(last_d);
  
  // set the links for surrounding nodes
  //
  if (last_d != (NODE*)NULL) {
    new_node->getPrev()->setNext(new_node);
  }
  
  // if the list was previously empty then set first, last, and current
  // to the new node. otherwise, just set the last node to the new node
  //
  if (isEmpty()) {
    first_d = new_node;
  }
  last_d = new_node;
  curr_d = last_d;
  
  // increment the size of the list
  //
  length_d += 1;
  
  // exit gracefully
  //
  return true;
}

// method: insertLast
//
// arguments:
//  DoubleLinkedList<TObject>& ilist: (input) the list to be inserted into
//                                    this list
//  
// return: a bool8 value indicating status
//
// this method adds a list into the end of this list
//
template<class TObject>
bool8
DoubleLinkedList<TObject>::insertLast(DoubleLinkedList<TObject>& ilist_a) {
  
  // if the input list is empty then there is nothing to do
  //
  if (ilist_a.isEmpty()) {
    
    // exit gracefully
    //
    return true;
  }

  // start at the front of the input list
  //
  NODE* tmp_node = ilist_a.first_d;
  
  // loop over each element in the input list and add each element to the list
  //
  while (tmp_node != (NODE*)NULL) {
    
    // add the next element to the list
    //
    insertLast(tmp_node->getItem());

    // move to the next node
    //
    tmp_node = tmp_node->getNext();
  }
  
  // exit gracefully
  //
  return true;
}

// method: removeLast
//
// arguments:
//  TObject*& item: (output) the item in the last node
//  
// return: a bool8 value indicating status
//
// this method removes the last node and outputs the item inside it. it is
// used only if the memory is USER-allocated. the user is forced to
// accept a pointer to the item so they know the memory is theirs to delete
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::removeLast(TObject*& item_a) {

  // make sure the last node is valid
  //
  if (last_d == (NODE*)NULL) {
    return false;
  }

  // make sure memory is allocated if we are SYSTEM-allocated
  //
  if ((alloc_d == SYSTEM) && (item_a == (TObject*)NULL)) {
    return (Error::handle(name(), L"removeLast", Error::NULL_ARG,
			  __FILE__, __LINE__));
  }

  // else initialize the output argument
  //
  else if (alloc_d == USER) {
    item_a = (TObject*)NULL;
  }

  // keep the position of the last node
  //
  NODE* tmp_node = last_d;

  // unlink the last node and set the new last node
  //  if the first node is not equal to the last node then there must be
  //  nodes in between. otherwise, the new list will be empty
  //
  if (last_d != first_d) {
    last_d->getPrev()->setNext((NODE*)NULL);
    last_d = last_d->getPrev();
  }
  else {
    first_d = (NODE*)NULL;
    last_d = (NODE*)NULL;
    curr_d = (NODE*)NULL;
  }

  // if the current node just got deleted then set the new current to be
  // the new last node
  //
  if (curr_d == tmp_node) {
    curr_d = last_d;
  }

  // if the marked node just got deleted then set it to NULL
  //
  if (mark_d == tmp_node) {
    mark_d = (NODE*)NULL;
  }

  // if allocation mode is USER, just return the pointer
  //
  if (alloc_d == USER) {
    item_a = tmp_node->getItem();
  }

  // for system mode delete the item memory
  //
  else {
    item_a->assign(*(tmp_node->getItem()));
    delete tmp_node->getItem();
  }

  // cleanup the node
  //
  delete tmp_node;
  length_d -= 1;

  // exit gracefully
  //
  return true;
}

// method: removeLast
//
// arguments: none
//  
// return: a bool8 value indicating status
//
// this method removes the last node. it is used only if the memory is
// SYSTEM-allocated. it cleans up all the memories in the last node. 
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::removeLast() {
  
  // make sure the last node is valid
  //
  if (last_d == (NODE*)NULL) {
    
    // return an error
    //
    return false;
  }

  // keep the position of the last node
  //
  NODE* tmp_node = last_d;
  
  // unlink the last node and set the new last node
  //  if the first node is not equal to the last node then there must be
  //  nodes in between. otherwise, the new list will be empty
  //
  if (last_d != first_d) {
    
    last_d->getPrev()->setNext((NODE*)NULL);
    last_d = last_d->getPrev();
  }
  else {
    first_d = (NODE*)NULL;
    last_d = (NODE*)NULL;
    curr_d = (NODE*)NULL;
  }
  
  // if the current node just got deleted then set the new current to be
  // the new last node
  //
  if (curr_d == tmp_node) {
    curr_d = last_d;
  }

  // if the marked node just got deleted then set it to NULL
  //
  if (mark_d == tmp_node) {
    mark_d = (NODE*)NULL;
  }

  // for SYSTEM mode delete the item memory
  //
  if (alloc_d == SYSTEM) {
    delete tmp_node->getItem();
  }
  
  // cleanup the old node
  //
  delete tmp_node;
  length_d -= 1;
  
  // exit gracefully
  //
  return true;
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  property methods
//
//---------------------------------------------------------------------------

// method: contains
//
// arguments:
//  TObject* item: (input) the item to be found
//
// return: a bool8 value indicating status
//
// this method determines if an item is contained in this list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::contains(const TObject* item_a) const {

  // check if the input item is NULL
  //
  if (item_a == (TObject*)NULL) {
    return Error::handle(name(), L"contains", Error::NULL_ARG,
			 __FILE__, __LINE__);
  }

  // save the current position
  //
  NODE* temp_curr = curr_d;

  // call the find method
  //
  bool8 item_found =
    const_cast<DoubleLinkedList<TObject>*>(this)->find(item_a);

  // revert the current pointer so as to leave the list unchanged
  //
  const_cast<DoubleLinkedList<TObject>*>(this)->curr_d = temp_curr;
  
  // return whether or not the item was found
  //
  return item_found;
}

// method: find
//
// arguments:
//  TObject* item: (input) the item to be found
//
// return: a bool8 value indicating status
//
// this method finds the first item in the list which is equivalent to the
// item passed in. if no equivalent item is found, false is returned. if an
// item is found, the list is positioned to that point, otherwise the state
// of the list is unchanged. 
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::find(const TObject* item_a) {
  
  // check if the input item is NULL
  //
  if (item_a == (TObject*)NULL) {
    return Error::handle(name(), L"find", Error::NULL_ARG,
			 __FILE__, __LINE__);
  }

  // save the current position
  //
  NODE* temp_curr = curr_d;

  // temporary variables
  //
  bool8 item_found = false;
  bool8 more_nodes = gotoFirst();
  
  // search from the beginning for the item
  //
  while (!item_found && more_nodes) {
    if (!(item_found = item_a->eq(*getCurr()))) {
      more_nodes = gotoNext();
    }
  }

  if (!item_found) {
    curr_d = temp_curr;
  }
  
  // return whether or not the item was found
  //
  return item_found;
}

// method: getPosition
//
// arguments: none
//
// return: the index position of the current element
//
// this method determines the ordinal index of the current
// element. assuming the list was unchanged a later call to
// gotoNode(pos) will return the current pointer to the current state.
//
template<class TObject>
int32 DoubleLinkedList<TObject>::getPosition() const {

  // if there are no elements, the position is invalid
  //
  if (isEmpty()) {
    return -1;
  }
  
  // save the current position
  //
  NODE* temp_curr = curr_d;

  // temporary variables
  //
  
  // search from the beginning for the item
  //
  int32 count = 0;
  for (bool8 more=const_cast<DoubleLinkedList<TObject>*>(this)->gotoFirst();
       more && (curr_d != temp_curr);
       more = const_cast<DoubleLinkedList<TObject>*>(this)->gotoNext()) {
    count++;
  }

  // restore the current pointer
  //
  const_cast<DoubleLinkedList<TObject>*>(this)->curr_d = temp_curr;

  // return whether or not the item was found
  //
  return count;
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  ordering methods
//
//---------------------------------------------------------------------------  

// method: sort
//
// arguments:
//  Integral::ORDER sort_order: (input) the order to sort
//  SORT_ALGO sort_algo_a: (input) the algorithm to use
// 
// return: a bool8 value indicating status
//
// this method sorts the elements in the list according to the given order
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::sort(Integral::ORDER sort_order_a,
					SORT_ALGO sort_algo_a) {

  // branch on sort algorithms
  //
  if (sort_algo_a == RAND_QUICK) {
    return randQuickSort(sort_order_a);
  }
  else if (sort_algo_a == INSERTION) {
    return insertionSort(sort_order_a);
  }
  else {
    return Error::handle(name(), L"sort", Error::NOT_IMPLEM,
                         __FILE__, __LINE__, Error::WARNING);
  }
}

// method: reverse
//
// arguments: none
//  
// return: a bool8 value indicating status
//
// this method reverse the order of element in the list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::reverse() {

  // if the list is empty then do nothing
  //
  if (isEmpty()) {
    return true;
  }
  
  // create temporary variables to use for exchanging pointers
  //
  NODE* tmp_node = first_d;
  NODE* tmp_next = (NODE*)NULL;
  
  // start from the beginning of the list, loop over each node and
  // reverse the link
  //
  while (tmp_node != (NODE*)NULL) {
    
    // exchange the previous and next pointers
    //
    tmp_next = tmp_node->getNext();
    tmp_node->setNext(tmp_node->getPrev());
    tmp_node->setPrev(tmp_next);

    // move to the next node for reversing
    //
    tmp_node = tmp_node->getPrev();
  }
  
  // exchange the old first and last nodes
  //
  tmp_node = first_d;
  first_d = last_d;
  last_d = tmp_node;
  
  // exit gracefully
  //
  return true;
}

// method: swap
//
// arguments:
//  int32 i: (input) the node to swap
//  int32 j: (input) the node to swap
//
// return: a bool8 value indicating status
//
// this method swaps the two nodes in the list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::swap(int32 i_a, int32 j_a) {
  
  // make sure the indices are in the range of the list
  //
  if (i_a < 0 || (i_a > (int32)length_d - 1) ||
      j_a < 0 || (j_a > (int32)length_d - 1)) {
    
    return Error::handle(name(), L"swap", Error::BOUNDS,
			 __FILE__, __LINE__);
  }
  
  // the private exchange method works for i < j,
  // so exchange the pointer if j < i
  //
  int32 i, j;
  if (i_a < j_a) {
    i = i_a;
    j = j_a;
  }
  else if ( j_a < i_a) {
    i = j_a;
    j = i_a;
  }
  else {
    
    // the same node, no need to exchange
    //
    return true;
  }

  // find the i_node and j_node
  //
  NODE* i_node = (NODE*)NULL;
  NODE* j_node = (NODE*)NULL;
  NODE* pp = first_d;
  
  for (int32 k = 0; k < i; k++) {
    pp = pp->getNext();
  }
  i_node = pp;
  
  for (int32 k = i; k < j; k++) {
    pp = pp->getNext();
  }
  j_node = pp;
  
  // call the other exchange method
  //
  return swap(i_node, j_node);
}

//---------------------------------------------------------------------------
//
// class-specific public methods:
//  apply methods
//
//---------------------------------------------------------------------------

// method: apply
//
// arguments:
//  bool8 (TObject::*method)(): (input) the method to apply to each element
//                                this method must be a member of the TObject
//                                class
//
// return: a bool8 value indicating status
//
// this method applies the input method to each element in the list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::apply(bool8 (TObject::*method_a)()) {

  // declare a return value
  //
  bool8 return_val = true;

  // create temporary variables
  //
  NODE* tmp_node = first_d;
  TObject* element;

  // loop over each element in the list and apply the method to it
  //
  while (tmp_node != (NODE*)NULL) {

    if ((element = tmp_node->getItem()) != (TObject*)NULL) {
      return_val &= ((tmp_node->getItem())->*method_a)();
    }
    else {
      return (Error::handle(name(), L"apply", Node<TObject>::ERR_NOITEM,
			    __FILE__, __LINE__));
    }

    // move to the next node
    //
    tmp_node = tmp_node->getNext();
  }
  
  // exit gracefully
  //
  return return_val;
}

// method: apply
//
// arguments:
//  bool8 (TObject::*method)(): (input) the method to apply to each element
//                                this method must be a member of the TObject
//                                class
//  DoubleLinkedList<TObject>& arg: (input) the list to be applied the
//                                  method on
//
// return: a bool8 value indicating status
//
// this method applies the input method to each element in the list
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::apply(bool8 (TObject::*method_a)(),
					 DoubleLinkedList<TObject>& arg_a) {
  
  // assign the input list to this linked list
  //
  assign(arg_a);
  
  // apply the method to the input list
  //
  return apply(method_a);
}

//---------------------------------------------------------------------------
//
// private methods
//
//---------------------------------------------------------------------------

// method: randQuickSort
//
// arguments:
//  Integral::ORDER sort_order: (input) ascending or descending
//
// return: a bool8 value indicating status
//
// "randomized version of quicksort" is taken from:
//
//  T. Cormen, C. Leiserson, R. Rivest,
//  Introduction to Algorithms, MIT Press,
//  Boston, Massachusetts, USA, pp. 153-171, 1998.
//
// this method sorts the linked list using the randomized sort algorithm
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::randQuickSort(Integral::ORDER
						 sort_order_a) {

  // declare the sort order to be true if the order is ascending and
  // false if the order is descending - this will help us in deciding
  // that whether we want to go to right or left
  //
  bool8 order = true;
  if (sort_order_a == Integral::DESCENDING) {
    order = false;
  }
  
  // declare two pairs of stacks which stores the pointers to nodes and their
  // indices in the linked list
  //
  Stack<NODE> p_stack(USER);
  Stack<NODE> r_stack(USER);
  Stack<Long> p_ind_stack(SYSTEM);
  Stack<Long> r_ind_stack(SYSTEM);

  
  // store the partition indices in the vectors
  //
  Long num;
  p_stack.push(first_d);
  num = 0;
  p_ind_stack.push(&num);
  
  r_stack.push(last_d);
  num = (int32)length_d - 1;
  r_ind_stack.push(&num);
  
  // declare the temporary variables
  //
  Long p;
  Long r;
  NODE* p_node;
  NODE* r_node;
  NODE* i_node;
  NODE* j_node;
  int32 i, j;

  TObject* tmp_obj;
  TObject pivot;
  
  // keep sorting the partition regions
  //
  while (!p_stack.isEmpty()) {
    
    p_node = (NODE* )NULL;
    r_node = (NODE* )NULL;
 
    p_node = p_stack.pop();
    r_node = r_stack.pop();
    
    p_ind_stack.pop(&p);
    r_ind_stack.pop(&r);
    
    // this is the partition method
    //
    if (p < r) {

      // select a number at random between p and r
      //
      Long rand_pivot = p;
      rand_pivot.rand(p, r);
      
      // error if the value does not lie between p and r
      //
      if ((rand_pivot < p) || (rand_pivot > r)) {
	return Error::handle(name(), L"randQuickSort", Error::ARG,
			     __FILE__, __LINE__);
      }
      
      // find the pivot element
      //
      curr_d = p_node;
      for (int32 k = p; k < rand_pivot; k++) {
	gotoNext();
      }
      tmp_obj = getCurr();
      pivot.assign(*tmp_obj);                    // pivot element
   
      // starting positions for sort, this is following the algorithm in the
      // reference, but since a linked list can not have indices < 0 or
      // > length_d, special cases of the start and end nodes have to be
      // taken care of
      //
      i = (int32)p - 1;
      j = (int32)r + 1;

      bool8 inode_is_first = false;
      bool8 jnode_is_last = false;
      
      if (p_node == first_d) {
	i_node = p_node;
	inode_is_first = true;
      }
      else {
	i_node = p_node->getPrev();
      }

      if (r_node == last_d) {
	j_node = r_node;
	jnode_is_last = true;
      }
      else {
	j_node = r_node->getNext();
      }
      
      // partition
      //
      do {

	// decrement j, increment i, until
        //   obj [i] >= x >= obj [j] for ascending
        //   obj [i] <= x <= obj [j] for desending
        //
	do {
	  
	  if (!jnode_is_last) {
	    j_node = j_node->getPrev();
	  }
	  j--;
	  tmp_obj = j_node->getItem();
	  jnode_is_last = false;
        } while ((order && pivot.lt(*tmp_obj)) ||
		 (!order && pivot.gt(*tmp_obj)));

	// increment i,
	//
	do {
	  if (!inode_is_first) {
	    i_node = i_node->getNext();
	  }
          i++;
	  tmp_obj = i_node->getItem();
	  inode_is_first = false;
	} while ((order && pivot.gt(*tmp_obj)) ||
		 (!order && pivot.lt(*tmp_obj)));
      
	// exchange
	//
        if (i < j) {
	  
	  // exchange the pointers to i and j nodes, if p, r nodes happen to
	  // be the same as i, j nodes, they need exchange also
	  //
	  if (r_node == j_node) {
	    r_node = i_node;
	  }
	  
	  if (p_node == i_node) {
	    p_node = j_node;
	  }
	  
	  swap(i_node, j_node);
	}
      } while (i < j);
      
      // now add p & r values to the vectors, similar to making new
      // function calls but with much less overhead
      //
      // randQuicksort2(A, p, j);
      //
      p_stack.push(p_node);
      r_stack.push(j_node);

      num = p;
      p_ind_stack.push(&num);
      num = j;
      r_ind_stack.push(&num);
      
      // randQuicksort2(A, j+1, r);
      //
      NODE* next_node =  j_node->getNext();
      p_stack.push(next_node);
      r_stack.push(r_node);
      
      num = j + 1;
      p_ind_stack.push(&num);
      num = r;
      r_ind_stack.push(&num);
    }
  }

  // free allocated memory
  //
  p_stack.clear(Integral::FREE);
  r_stack.clear(Integral::FREE);
  p_ind_stack.clear(Integral::FREE);
  r_ind_stack.clear(Integral::FREE);

  // exit gracefully
  //
  return true;
}

// method: insertionSort
//
// arguments:
//  Integral::ORDER sort_order: (input) ascending or descending
//
// return: a bool8 value indicating status
//
// this method sorts the linked list using the insertion sort algorithm
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::insertionSort(Integral::ORDER
						 sort_order_a) {

  // declare local variables
  //
  bool8 status = false;
  
  // if empty or only 1 item, no need to sort
  //
  if ((int32)length_d == 0 || (int32)length_d == 1) {
    return true;
  }
  
  // local variables
  //
  TObject* insert_item;
  TObject* curr_item;
  bool8 cont = true;
  
  NODE* insert_node_next = (NODE*)NULL;
  NODE* insert_node_prev = (NODE*)NULL;
  NODE* insert_node = (NODE*)NULL;
  NODE* prev_node = (NODE*)NULL;
  
  // insert starting from the second node
  //
  insert_node = first_d->getNext();

  for (int32 j = 1; j < (int32)length_d; j++) {
    
    insert_item = insert_node->getItem();
    
    // insert node (j) into the sorted nodes node [0, ... , j -1]
    // 
    curr_d = first_d;
    
    for (int32 i = 0; (i < j) && cont; i++) {

      // remember the next node of the current insertion node
      //
      insert_node_next = insert_node->getNext();
      
      // compare the insert node with the current node
      //
      curr_item = getCurr();

      // determine the sort order i.e., Integral::ASCENDING or
      // Integral::DESCENDING
      //
      if (sort_order_a == Integral::ASCENDING) {
	status = insert_item->lt(*curr_item);
      }
      else {
	status = insert_item->gt(*curr_item);
      }
	
      if (status) {
	cont = false;
	
	// disconnect the insert node
	//
	insert_node_prev = insert_node->getPrev();
	if (insert_node_prev != (NODE*)NULL) {
	  insert_node_prev->setNext(insert_node_next);
	}
	
	if (insert_node_next != (NODE*)NULL) {
	  insert_node_next->setPrev(insert_node_prev);
	}
	else {
	  last_d = insert_node_prev;
	}
	
	// insert in front of the curr_d
	//
	prev_node = curr_d->getPrev();
	if (prev_node != (NODE*)NULL) {
	  prev_node->setNext(insert_node);
	  insert_node->setPrev(curr_d->getPrev());
	}
	else {
	  first_d = insert_node;
	  insert_node->setPrev((NODE*)NULL);
	}
	  
	insert_node->setNext(curr_d);
	curr_d->setPrev(insert_node);
      }

      // move the current node forward
      //
      gotoNext();
    }
    
    // go to the next insertion node
    //
    insert_node = insert_node_next;
    cont = true;
  }
  
  // exit gracefully
  //
  return true;
}

// method: swap
//
// arguments:
//  NODE* i_node: (input) the node to swap
//  NODE* j_node: (input) the node to swap
//
// return: a bool8 value indicating status
//
// this method swaps two nodes in the list. it assumes that the
// nodes are in the list and in the proper order. this avoids
// the overhead of checking. this is a private method, and is
// used inside sort and the other swap method.
//
template<class TObject>
bool8 DoubleLinkedList<TObject>::swap(NODE*& i_node_a,
					NODE*& j_node_a) {

  // declare local variables
  //
  NODE* tmp_node;

  // get the connections of i, j nodes
  //
  NODE* j_prev_node = j_node_a->getPrev();
  NODE* j_next_node = j_node_a->getNext();
  NODE* i_prev_node = i_node_a->getPrev();
  NODE* i_next_node = i_node_a->getNext();

  // reconnect i_node_a and j_node_a (take care of the case that i_node_a and
  // j_node_a are adjacent, or the first / last node)
  //
  if (i_next_node == j_node_a) {
    i_next_node = i_node_a;
    j_prev_node = j_node_a;
  }
  
  j_node_a->setPrev(i_prev_node);
  if (i_prev_node != (NODE*)NULL) {
    i_prev_node->setNext(j_node_a);
  }
  else {
    first_d = j_node_a;
  }
  
  j_node_a->setNext(i_next_node);
  if (i_next_node != (NODE*)NULL) {
    i_next_node->setPrev(j_node_a);
  }
  else {
    last_d = j_node_a;
  }
  
  i_node_a->setPrev(j_prev_node);
  if (j_prev_node != (NODE*)NULL) {
    j_prev_node->setNext(i_node_a);
  }
  else {
    first_d = i_node_a;
  }
  
  i_node_a->setNext(j_next_node);
  if (j_next_node != (NODE*)NULL) {
    j_next_node->setPrev(i_node_a);
  }
  else {
    last_d = i_node_a;
  }
	  
  // exchange the pointers to i and j nodes, 
  //
  tmp_node = j_node_a;
  j_node_a = i_node_a;
  i_node_a = tmp_node;
  
  // exit gracefully
  //
  return true;
}

// method: getNode
//
// arguments:
//  int32 index: (input) the index with respect to the first_d
//
// return: the NODE
//
// this method gets the node of the given index in the list
//
template<class TObject>
DoubleLinkedNode<TObject>* DoubleLinkedList<TObject>::getNode(int32 index_a) const {

  // make sure the index is in the valid range
  //
  if ((index_a < -1) || (index_a >= length_d)) {
    Error::handle(name(), L"getNode", Error::BOUNDS, __FILE__, __LINE__);
    return (NODE*)NULL;
  }

  NODE* pp = (NODE*)NULL;
  
  // if we are in the first half, search from the start, else search
  // from the end.
  //
  if (index_a <= ((int32)length_d / 2)) {
    
    // start at the start and work forward
    //
    pp = first_d;
    
    for (int32 i = 0; i < index_a; i++) {
      pp = pp->getNext();
    }
  }
  else {

    // start at the end and work backwards
    //
    pp = last_d;
    
    for (int32 i = 0; i < (length_d - index_a - 1); i++) {
      pp = pp->getPrev();
    }
  }
  
  // return the node
  //
  return pp;
}

// end of include file
//
#endif
