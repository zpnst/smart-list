
# include <cmath>
# include <memory>
# include <cstdint>
# include <iostream>


template <typename PointerType>
using SP = std::shared_ptr<PointerType>;

template <typename PointerType>
using WP = std::weak_ptr<PointerType>;


template <typename ListType>
class List final 
{

    /* The structure on which all the
       elements of the list are built */
    struct ListNode
    {   
        ListType Value;
        SP<ListNode> Next = nullptr;
        WP<ListNode> Previous;
    };

    std::uint64_t Size = 0;
    SP<ListNode> Muzzle = nullptr;
    SP<ListNode> Tail = nullptr;

    /* Private function auxiliary to the plenty function */
    bool is_in_cache(std::uint64_t Value, List<ListType> Cache) {
 
        SP<ListNode> CurrentNode = Cache.Muzzle;
        for (std::uint64_t iter = 0; iter < Cache.Size; iter++) {
            if (CurrentNode -> Value == Value) return true; 
            CurrentNode = CurrentNode -> Next;
        }
        return false;

    }
    public:
        
        /* Default Constructor & Destructor */
        List(void) = default;
        ~List(void) = default;

        /* Constructor based on parametr pack */
        template <typename ... Arguments>
        List(Arguments ... arguments) noexcept {
            (push_back(arguments), ...);
        }

        /* Constructor based on std::initializer_list */
        List(const std::initializer_list<ListType>& Data) noexcept {

            for (const ListType& Object : Data) {
                push_back(Object);
            }
        } 
        
        /* Function for adding an item to any place in the list */
        List& insert(const std::uint64_t Index, const ListType& Data) {

            SP<ListNode> NewNode = std::make_shared<ListNode>();
            NewNode -> Value = Data;
            
            Size += 1;
            NewNode -> Next = nullptr;

            if (Index > Size - 1 || Index < 0)  [[ unlikely ]] {
                throw std::out_of_range("ERROR! List index out of range!");
            }

            if (Muzzle == nullptr) [[ unlikely ]] {
                Muzzle = NewNode;
                Tail = NewNode;
                return *this;
            }

            if (Index == 0) [[ likely ]] {
                NewNode -> Next = Muzzle;
                Muzzle -> Previous = NewNode;
                Muzzle = NewNode;
                return *this;
            }

            if (Index == Size - 1) [[ likely ]] {
                Tail -> Next = NewNode;
                NewNode -> Previous = Tail;
                Tail = NewNode;
                return *this;
            }

            SP<ListNode> CurrentNode = Muzzle;

            for (std::uint64_t iter = 0; iter < Index - 1; iter++)
                CurrentNode = CurrentNode -> Next;

            NewNode -> Next = CurrentNode -> Next;
            CurrentNode -> Next -> Previous = NewNode;
            CurrentNode -> Next = NewNode;
            NewNode -> Previous = CurrentNode;

            return *this;
        }

        /* Function for deleting an item anywhere in the list */
        List& erase(const std::uint64_t Index) {

            Size -= 1;

            if (Index > Size || Index < 0) [[ unlikely ]] {
                throw std::out_of_range("ERROR! List index out of range!");
            }

            if (Muzzle == nullptr) [[ unlikely ]] {
                throw std::out_of_range("ERROR! The list has no elements!");
            }

            if (Index == 0) [[ likely ]] {
                SP<ListNode> TemporaryNode = Muzzle -> Next;
                TemporaryNode -> Previous.lock() = nullptr;
                Muzzle = TemporaryNode;
                return *this;
            }

            if (Index == Size) [[ likely ]] {
                SP<ListNode> TemporaryNode = Tail -> Previous.lock();
                TemporaryNode -> Next = nullptr;
                Tail = TemporaryNode;
                return *this;
            }
            
            SP<ListNode> CurrentNode = Muzzle;
            for (std::uint64_t iter = 0; iter < Index - 1; iter++)
                CurrentNode = CurrentNode -> Next;

            SP<ListNode> TemporaryNode = CurrentNode -> Next -> Next;
            CurrentNode -> Next = TemporaryNode;
            TemporaryNode -> Previous = CurrentNode;
            return *this;
        }

        /* Function for creating a set from a list */
        List& plenty() {
            
            if (Muzzle == nullptr) [[ unlikely ]] {
                throw std::out_of_range("ERROR! The list has no elements!");
            }

            List<ListType> Cache;
            std::uint64_t parametr = 0;
            std::uint64_t CurrentSize = Size;

            SP<ListNode> CurrentNode = Muzzle;
            
            for (std::uint64_t iter = 0; iter < CurrentSize; iter++) {
                if (is_in_cache(CurrentNode -> Value, Cache) == false) 
                    Cache.push_back(CurrentNode -> Value);
                else 
                    erase(iter - parametr++);

                CurrentNode = CurrentNode -> Next;
            }
            return *this;
    
        }

        /* CONVENIENT FUNCTIONS THAT ARE BASED ON 
           ALREADY EXISTING FUNCTIONS */

        List& push_front(const ListType& Data) noexcept {
            
            insert(0, Data);
            return *this;
        }

        List& push_back(const ListType& Data) noexcept {

            insert(Size, Data);
            return *this;
        }

        List& pop_back(void) noexcept {
            
            erase(Size - 1);
            return *this;
        }

        List& pop_front(void) noexcept{

            erase(0);
            return *this;
        }
        
        /* Function for sorting the list based on bubble sort */
        List& sort() {

            SP<ListNode> CurrentNode = Muzzle;
            
            for (std::uint64_t iter = 0; iter < Size - 1; iter++) {
                for (std::uint64_t jter =0; jter < Size - iter - 1; jter++) {
                    if (CurrentNode -> Value > CurrentNode -> Next -> Value) 
                    {
                        std::uint64_t temporary = CurrentNode -> Value;
                        CurrentNode -> Value = CurrentNode -> Next -> Value;
                        CurrentNode -> Next -> Value = temporary;
                    }
                    CurrentNode = CurrentNode -> Next;
                }
                CurrentNode = Muzzle;
            }

            return *this;
        }

        /* Operator [] overload */
        ListType& operator [] (const std::uint64_t Index) {

            if (Index > Size || Index < 0) [[ unlikely ]] {
                throw std::out_of_range("List index out of range!");
            }

            if (Index == 0) [[ unlikely ]] {
                return Muzzle -> Value;
            }

            if (Index == Size - 1) [[ unlikely ]] {
                return Tail -> Value;
            }

            SP<ListNode> CurrentNode = Muzzle;
            for (std::uint64_t iter = 0; iter < Index; iter++) 
                CurrentNode = CurrentNode -> Next;

            return CurrentNode -> Value;
            
        }

        /* Function for formatted list output */
        List& print_list(void) noexcept {

            SP<ListNode> CurrentNode = Muzzle;
            std::cout << "nullptr <--> ";
            
            while(CurrentNode != nullptr) {   
                std::cout << CurrentNode -> Value << " <--> ";
                CurrentNode = CurrentNode -> Next;
            }
            std::cout << "nullptr" << std::endl;
            return *this;
        }

        inline std::uint64_t size(void) noexcept {
            return Size;
        }

};


/* OPERATORS OVERLOADING: (*, +, /) */

/* List<ListType> * Variable */
template <typename ListType, typename ValueType>
requires std::is_same_v<ListType, ValueType>
[[ nodiscard ]] List<ListType> operator * (List<ListType> CurrentList, ValueType Value) {
    
    List<ListType> Result;
    for (unsigned int iter = 0; iter < CurrentList.size(); iter++)
        Result.push_back(CurrentList[iter] * Value);
    return Result;
}

/* List1<ListType> * List2<ListType> */
template <typename ListType1, typename ListType2>
requires std::is_same_v<ListType1, ListType2>
[[ nodiscard ]] List<ListType1> operator * (List<ListType1> CurrentList1, List<ListType2> CurrentList2) {
    
    List<ListType1> Result;
    std::uint64_t MinSize = std::min<std::uint64_t>(CurrentList1.size(), CurrentList2.size());
    for (unsigned int iter = 0; iter < MinSize; iter++) 
        Result.push_back(CurrentList1[iter] * CurrentList2[iter]);

    if (CurrentList1.size() > CurrentList2.size())
        for (unsigned int iter = MinSize; iter < CurrentList1.size(); iter++)
            Result.push_back(CurrentList1[iter]);
    else
        for (unsigned int iter = MinSize; iter < CurrentList2.size(); iter++)
            Result.push_back(CurrentList2[iter]); 

    return Result;
}

/* List<ListType> + Variable */
template <typename ListType, typename ValueType>
requires std::is_same_v<ListType, ValueType>
[[ nodiscard ]] List<ListType> operator + (List<ListType> CurrentList, ValueType Value) {
    
    List<ListType> Result;
    for (unsigned int iter = 0; iter < CurrentList.size(); iter++) 
        Result.push_back(CurrentList[iter] + Value);
    return Result;
}

/* List1<ListType> + List2<ListType> */
template <typename ListType1, typename ListType2>
requires std::is_same_v<ListType1, ListType2>
[[ nodiscard ]] List<ListType1> operator + (List<ListType1> CurrentList1, List<ListType2> CurrentList2) {
    
    List<ListType1> Result;
    std::uint64_t MinSize = std::min<std::uint64_t>(CurrentList1.size(), CurrentList2.size());
    for (unsigned int iter = 0; iter < MinSize; iter++) 
        Result.push_back(CurrentList1[iter] + CurrentList2[iter]);

    if (CurrentList1.size() > CurrentList2.size())
        for (unsigned int iter = MinSize; iter < CurrentList1.size(); iter++)
            Result.push_back(CurrentList1[iter]);
    else
        for (unsigned int iter = MinSize; iter < CurrentList2.size(); iter++)
            Result.push_back(CurrentList2[iter]); 

    return Result;
}

/* List<ListType> / Variable */
template <typename ListType, typename ValueType>
requires std::is_same_v<ListType, ValueType>
[[ nodiscard ]] List<ListType> operator / (List<ListType> CurrentList, ValueType Value) {
    
    List<ListType> Result;
    if (Value == 0) 
        throw std::logic_error("Zero division error!");
    for (unsigned int iter = 0; iter < CurrentList.size(); iter++) 
        Result.push_back(CurrentList[iter] / Value);
    return Result;
}

/* List1<ListType> / List2<ListType> */
template <typename ListType1, typename ListType2>
requires std::is_same_v<ListType1, ListType2>
[[ nodiscard ]] List<ListType1> operator / (List<ListType1> CurrentList1, List<ListType2> CurrentList2) {
    
    List<ListType1> Result;
    std::uint64_t MinSize = std::min<std::uint64_t>(CurrentList1.size(), CurrentList2.size());
    for (unsigned int iter = 0; iter < MinSize; iter++) 
        Result.push_back(CurrentList1[iter] / CurrentList2[iter]);

    if (CurrentList1.size() > CurrentList2.size())
        for (unsigned int iter = MinSize; iter < CurrentList1.size(); iter++)
            Result.push_back(CurrentList1[iter]);
    else
        for (unsigned int iter = MinSize; iter < CurrentList2.size(); iter++)
            Result.push_back(CurrentList2[iter]); 

    return Result;
}

int main(int argc, char* argv[]) {

    /* 1) Creating object of class List using default Constructor
       2)  Creating object of class List using parametr pack
       3) Creating object of class List using std::inizializer_lis */

    List<std::uint64_t> list1;
    List<std::uint64_t> list2(1, 2, 3, 4, 5);
    List<std::uint64_t> list3 = {4, 5};

    list3.print_list();

    
    /* Using methods push_back & push_front in the loop */
    for (std::int64_t iter = 6; iter <  7; iter++) list3.push_back(iter);
    for (std::int64_t iter = 3; iter >= 1; iter--) list3.push_front(iter);

    /* Using method insert */
    list3.insert(list3.size(), 999);

    /* Since the methods return a reference to an instance of
       the List class, we can use them as follows: */
    list3.push_back(991).push_back(992).push_front(101).push_front(102);

    /* Using method erase */
    list3.erase(2).erase(0).erase(1);
   
    /* Using methods pop_back & pop_front */
    list3.pop_back().pop_front();

    /* Printing list and list size */
    list3.print_list();
    std::cout << "List Size: " << list3.size() << std::endl;

    /* We can print the list in this way as well, since the
       [] operator is overloaded in the List class */
    for (std::uint64_t iter = 0; iter < list3.size(); iter++) {
        std::cout << list3[iter] << " ";
    }   
    std::cout << std::endl;

    /* Also, overloading of operators *, +, \ is implemented
       Consider an example with the operator / */
       
    List<double> listt1 = {1, 2, 3};
    List<double> listt2 = {4, 5, 6, 7, 8};

    /* The types must be the same */
    List<double> listt3 = listt1 / (double)10;
    List<double> listt4 = listt1 / listt2;

    listt3.print_list();
    listt4.print_list();

    /* We can make a set from the list by removing duplicate elements */
    List<int> list = {12, 36, 25, 45, 45, 23, 99, 1000, 45, 45, 89, 101, 110, 111, 112, 111};
    list.plenty();
    list.print_list();

    /* And sort the list */
    list.sort();
    list.print_list();
    
    /* EXAMPLE OF FUNCTIONALITY */
    List<int> __list = {5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5};
    __list.print_list().plenty().print_list().sort().print_list();
    std::cout << "List Size: " << __list.size() << std::endl;
    

    return 0;
}
