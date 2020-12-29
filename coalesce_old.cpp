#include <sys/mman.h>
#include <iostream>

class Coalesce
{
public:
    struct PageNode
    {
        PageNode(PageNode* _next, void* _memory)
        {
            next = _next;
            memory = _memory;
        }

        PageNode * next;
        void * memory;
        size_t free_block_index;
    };

    struct BlockData
    {
        BlockData(BlockData* _next,
                  BlockData* _prev,
                  void* _memory,
                  size_t _size,
                  bool _with_value)
        {
            next = _next;
            prev = _prev;
            memory = _memory;
            size = _size;
        }

        BlockData* next;
        BlockData* prev;
        void * memory;
        size_t size;
        bool with_value;
    };

    // Coalesce();

    // ~Coalesce();

    void init()
    {
        _inited = true;

        void* map = mmap(NULL, PageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        _first_block_data = new ((BlockData *)map) BlockData(nullptr, nullptr, map, PageSize - sizeof(BlockData), false);
        _count_of_blocks = 1;
    }

    // void destroy();

    void* alloc(size_t size)
    {
        std::cout << "sizeof(BlockData): " << sizeof(BlockData) << std::endl;

        void* ptr;

        BlockData * temp = _first_block_data;
        for (size_t i = 0; i < _count_of_blocks; i++)
        {
            std::cout << "i: " << i << std::endl;

            BlockData* temp_block_data;

            if (temp->with_value == false &&
                temp->size >= size)
            {
                _count_of_blocks++;
                ptr = (char*)temp->memory + sizeof(BlockData);
                
                temp->with_value = true;
                temp_block_data = new ((char*)ptr + size) BlockData(temp->next,
                                                  (BlockData*)temp->memory,
                                                  (void*)((char*)ptr + size),
                                                  temp->size - ((char*)ptr + size - (char*)temp->memory),
                                                  false);

                temp->size = size;
                temp->next = temp_block_data;
                return ptr;
            }

            temp = temp->next;
        }
        return ptr;
    }

    void free(void* p)
    {
        std::cout << "FREE" << std::endl;
        BlockData* temp = (BlockData*)((char*)p - sizeof(BlockData));
        temp->with_value = false;
        if (temp->next != nullptr && 
            temp->next->with_value == false)
        {
            // Вынести количество блоков под управлении страницы
            _count_of_blocks--;
            temp->size += temp->next->size + sizeof(BlockData);
            if (temp->next->next != nullptr)
            {
                temp->next->next->prev = temp;
            }
            temp->next = temp->next->next;
        }
        if (temp->prev != nullptr &&
            temp->prev->with_value == false)
        {
            _count_of_blocks--;
            temp->prev->size += temp->size + sizeof(BlockData);
            temp->prev->next = temp->next;
        }

        std::cout << "Value of block: " << *((int*)((char*)temp->memory + sizeof(BlockData))) << std::endl;
        std::cout << std::endl;
    }

    //TEST METHODS
    void show_data()
    {
        BlockData* temp = _first_block_data;
        for (size_t i = 0; i < _count_of_blocks; i++)
        {
            std::cout << "Count of block data: " << i + 1 << std::endl;
            std::cout << "Size of block info: " << temp->size << std::endl;
            std::cout << "Value of block: " << *((int*)((char*)temp->memory + sizeof(BlockData))) << std::endl;
            std::cout << "With value: " << temp->with_value << std::endl;
            std::cout << std::endl;

            temp = temp->next;
        }
    }


private:
    const static size_t PageSize = 512;
    bool _inited;
    size_t _count_of_blocks;
    size_t _number_of_pages;
    // Temp переменная для ссылки на первую страницу
    // void* map;
    BlockData* _first_block_data;
    PageNode* _current_page;
    PageNode* _first_page;
};