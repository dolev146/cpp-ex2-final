#include "Notebook.hpp"
constexpr int COL_LIMIT = 99;
constexpr int COL_AMOUNT = 100;
constexpr int SAFTEY_MUL = 3;
constexpr int SAFTEY_ADD = 5;
constexpr int NOT_VERTICAL = 0;
constexpr int ADD = 1;
constexpr int DEFAULT_PAGE_SIZE = 0;
constexpr size_t DEFAULT_CHAR_INDEX = 0;

// hours wasted: 72 my life i want to die please fast
// used searches
// https://stackoverflow.com/questions/3136520/determine-if-map-contains-a-value-for-a-key
// https://stackoverflow.com/questions/65877299/wsign-conversion-error-when-getting-index-for-vector
// https://www.learncpp.com/cpp-tutorial/unsigned-integers-and-why-to-avoid-them/
// https://stackoverflow.com/questions/191757/how-to-concatenate-a-stdstring-and-an-int
// https://stackoverflow.com/questions/4527686/how-to-update-stdmap-after-using-the-find-method
// https://stackoverflow.com/questions/44467268/how-to-extend-vector-size-by-one-element-and-fill-it-with-variable
// https://stackoverflow.com/questions/15889578/how-can-i-resize-a-2d-vector-of-objects-given-the-width-and-height
// old project that failed https://github.com/dolev146/CPP-EX2 but it doesnt work on large numbers

namespace ariel
{

    void negative_values_new_check(int const &page, int const &row, int const &column, int const &length, Direction dir);

    void check_page_not_negative(int page)
    {
        if (page < 0)
        {
            throw std::runtime_error("no negative page !");
        }
    }

    void check_printable_chars(std::string const &txt)
    {
        for (size_t i = 0; i < txt.size(); i++)
        {
            char c = txt.at(i);
            if (c < ' ' || c >= '~')
            {
                throw std::runtime_error("error cannot write \\n ");
            }
        }

        if (txt == "\n")
        {
            throw std::runtime_error("error cannot write \\n ");
        }
    }

    void bound_column_check(std::string const &txt, int column)
    {
        unsigned int size_of_str = txt.size();
        unsigned int u_column = unsigned(column);
        if (column > COL_LIMIT + ADD || size_of_str + u_column > COL_LIMIT + ADD)
        {
            throw std::invalid_argument("column can't be greater that 99 , bad index");
        }
    }

    void bound_column_check(int length, int column)
    {
        unsigned int u_length = unsigned(length);
        unsigned int u_column = unsigned(column);
        if (column > COL_LIMIT + ADD || u_length + u_column > COL_LIMIT + ADD)
        {
            throw std::invalid_argument("column can't be greater that 99 , bad index");
        }
    }

    void check_negative_values_write(int page, int row, int column)
    {
        if (page < 0 || row < 0 || column < 0)
        {
            throw std::runtime_error("negative value");
        }
    }

    void add_row_horizontal(std::map<int, std::map<int, std::string>>::iterator &it, std::map<int, std::string> &pageOfbook, int row, int column, std::string const &txt)
    {
        std::map<int, std::string>::iterator it2 = pageOfbook.find(row);
        if (it2 != pageOfbook.end())
        {
            // row found
            std::string row_of_page = it2->second;
            size_t u_column = (size_t)column;
            size_t size_of_txt = txt.size();
            size_t counter = 0;
            for (size_t i = u_column; i < u_column + size_of_txt; i++)
            {
                if (row_of_page[i] != '_')
                {
                    throw std::invalid_argument("index already taken ");
                }
                row_of_page[i] = txt[counter];
                counter++;
            }
            it2->second = row_of_page;
            it->second = pageOfbook;
        }
        else
        {
            // row not found need to create
            std::string row_of_page = std::string(COL_AMOUNT, '_');
            size_t counter = 0;
            size_t size_of_txt = txt.size();
            size_t u_column = (size_t)column;
            for (size_t i = u_column; i < u_column + size_of_txt; i++)
            {
                row_of_page[i] = txt[counter];
                counter++;
            }
            pageOfbook.insert(std::pair<int, std::string>(row, row_of_page));
            it->second = pageOfbook;
        }
    }

    void create_row_horizontal(std::string const &txt, int row, int column, int page, std::map<int, std::map<int, std::string>> &notebook)
    {
        size_t size_of_txt = txt.size();
        std::map<int, std::string> pageOfbook;
        std::string row_of_page = std::string(COL_AMOUNT, '_');
        size_t counter = 0;
        size_t u_column = (size_t)column;
        for (size_t i = u_column; i < u_column + size_of_txt; i++)
        {
            row_of_page[i] = txt[counter];
            counter++;
        }
        pageOfbook.insert(std::pair<int, std::string>(row, row_of_page));
        notebook.insert(std::pair<int, std::map<int, std::string>>(page, pageOfbook));
    }

    void create_empty_row_horizontal(int row, int column, int page, std::map<int, std::map<int, std::string>> &notebook)
    {
        std::map<int, std::string> pageOfbook;
        std::string row_of_page = std::string(COL_AMOUNT, '_');
        pageOfbook.insert(std::pair<int, std::string>(row, row_of_page));
        notebook.insert(std::pair<int, std::map<int, std::string>>(page, pageOfbook));
    }

    void create_row_vertical(std::string const &txt, int row, int column, std::map<int, std::map<int, std::string>> &notebook, int page)
    {
        size_t size_of_txt = txt.size();
        std::map<int, std::string> pageOfbook;
        size_t counter = 0;
        size_t u_column = (size_t)column;
        for (int i = 0; i < size_of_txt; i++)
        {
            std::string row_of_page = std::string(COL_AMOUNT, '_');
            row_of_page[u_column] = txt[counter];
            pageOfbook.insert(std::pair<int, std::string>(row + i, row_of_page));
            counter++;
        }
        notebook.insert(std::pair<int, std::map<int, std::string>>(page, pageOfbook));
    }

    void create_one_char_row(std::string const &txt, int column, int row, int counter, size_t i, std::map<int, std::string> &pageOfbook, std::map<int, std::map<int, std::string>>::iterator &it)
    {
        std::string c = std::string(1, txt.at(i));
        std::string row_of_page = std::string(COL_AMOUNT, '_');
        size_t u_column = (size_t)column;
        row_of_page.at(u_column) = txt.at(i);
        pageOfbook.insert(std::pair<int, std::string>(row + counter, row_of_page));
        it->second = pageOfbook;
    }

    /* read  ************************************************/

    void read_horizantal_create(int length, int row, int column, int page, std::map<int, std::map<int, std::string>> &notebook, std::string &output)
    {
        // horizontal read
        size_t size_of_txt = unsigned(length);
        std::map<int, std::string> pageOfbook;
        std::string row_of_page = std::string(COL_AMOUNT, '_');
        size_t u_column = (size_t)column;
        size_t counter = 0;
        for (size_t i = u_column; i < u_column + size_of_txt; i++)
        {
            output += row_of_page[counter];
            counter++;
        }
        pageOfbook.insert(std::pair<int, std::string>(row, row_of_page));
        notebook.insert(std::pair<int, std::map<int, std::string>>(page, pageOfbook));
    }

    void read_vertical_create(int length, int row, int column, int page, std::map<int, std::map<int, std::string>> &notebook, std::string &output)
    {
        size_t size_of_txt = unsigned(length);
        std::map<int, std::string> pageOfbook;
        size_t u_column = (size_t)column;
        for (int i = 0; i < size_of_txt; i++)
        {
            std::string row_of_page = std::string(COL_AMOUNT, '_');
            output += row_of_page[u_column];
            pageOfbook.insert(std::pair<int, std::string>(row + i, row_of_page));
        }
        notebook.insert(std::pair<int, std::map<int, std::string>>(page, pageOfbook));
    }

    void page_found_vertical_read(std::map<int, std::map<int, std::string>>::iterator &it, int row, int column, int length, std::string &output, int page, std::map<int, std::map<int, std::string>> &notebook)
    {
        std::map<int, std::string> pageOfbook = it->second;
        size_t u_length = unsigned(length);
        for (size_t i = 0; i < u_length; i++)
        {
            // if row doesnt exsists
            int counter = (int)i;
            std::map<int, std::string>::iterator it2 = pageOfbook.find(row + counter);
            if (it2 != pageOfbook.end())
            {
                // row found
                std::string row_of_page = it2->second;
                size_t u_column = (size_t)column;
                size_t size_of_txt = unsigned(length);
                output += row_of_page[u_column];
            }
            else
            {
                // row not found so we create
                size_t size_of_txt = unsigned(length);
                std::map<int, std::string> pageOfbook;
                size_t u_column = (size_t)column;
                std::string row_of_page = std::string(COL_AMOUNT, '_');
                output += row_of_page[u_column];
                pageOfbook.insert(std::pair<int, std::string>(row + (int)i, row_of_page));
                notebook.insert(std::pair<int, std::map<int, std::string>>(page, pageOfbook));
            }
        }
    }

    void page_found_horizontal_read(std::map<int, std::map<int, std::string>>::iterator &it, int row, int column, int length, std::string &output)
    {
        std::map<int, std::string> pageOfbook = it->second;
        // if row exists we will use it ,
        // else we need to make a row
        // fix this
        std::map<int, std::string>::iterator it2 = pageOfbook.find(row);
        if (it2 != pageOfbook.end())
        {
            // row found
            std::string row_of_page = it2->second;
            size_t u_column = (size_t)column;
            size_t size_of_txt = unsigned(length);
            for (size_t i = u_column; i < u_column + size_of_txt; i++)
            {
                output += row_of_page[i];
            }
        }
        else
        {
            // row not found need to create
            std::string row_of_page = std::string(COL_AMOUNT, '_');
            size_t size_of_txt = unsigned(length);
            size_t u_column = (size_t)column;
            for (size_t i = u_column; i < u_column + size_of_txt; i++)
            {
                output += row_of_page[i];
            }
            pageOfbook.insert(std::pair<int, std::string>(row, row_of_page));
            it->second = pageOfbook;
        }
    }
    /*************************************************************/

    void Notebook::write(int page, int row, int column, Direction dir, std::string const &txt)
    {
        check_printable_chars(txt);
        check_negative_values_write(page, row, column);
        if (column > COL_LIMIT)
        {
            throw std::runtime_error("nagetive values"); 
        }
        if (dir == Direction::Horizontal)
        {
            bound_column_check((int)txt.size(), column);
        }

        std::map<int, std::map<int, std::string>>::iterator it = notebook.find(page);
        if (it != notebook.end())
        {
            // page found
            if (dir == Direction::Horizontal)
            {
                // horizontal
                std::map<int, std::string> pageOfbook = it->second;
                // if row exists we will use it ,
                // else we need to make a row
                add_row_horizontal(it, pageOfbook, row, column, txt);
            }
            else
            {
                // vertical
                std::map<int, std::string> pageOfbook = it->second;
                size_t length = txt.size();
                for (size_t i = 0; i < length; i++)
                {
                    // if row doesnt exsists
                    int counter = (int)i;
                    std::map<int, std::string>::iterator it2 = pageOfbook.find(row + counter);
                    if (it2 != pageOfbook.end())
                    {
                        // row found
                        // it exists so we update the values
                        std::string c = std::string(1, txt[i]);
                        add_row_horizontal(it, pageOfbook, row + counter, column, c);
                    }
                    else
                    {
                        // row not found so we create
                        create_one_char_row(txt, column, row, counter, i, pageOfbook, it);
                    }
                }
            }
        }
        else
        {
            // page not found
            if (dir == Direction::Horizontal)
            {
                // horizontal
                create_row_horizontal(txt, row, column, page, notebook);
            }
            else
            {
                // vertical
                create_row_vertical(txt, row, column, notebook, page);
            }
        }
    }

    std::string Notebook::read(int page, int row, int column, Direction dir, int length)
    {
        negative_values_new_check(page, row, column, length, dir);
        if (column > COL_LIMIT)
        {
            throw std::runtime_error("nagetive values");
        }
        if (dir == Direction::Horizontal)
        {
            bound_column_check(length, column);
        }

        std::string output;
        std::map<int, std::map<int, std::string>>::iterator it = notebook.find(page);
        if (it != notebook.end())
        {
            // page found
            if (dir == Direction::Horizontal)
            {
                // horizontal
                page_found_horizontal_read(it, row, column, length, output);
            }
            else
            {
                // vertical
                page_found_vertical_read(it, row, column, length, output, page, notebook);
            }
        }
        else
        {
            // page not found
            if (dir == Direction::Horizontal)
            {
                // horizontal read
                read_horizantal_create(length, row, column, page, notebook, output);
            }
            else
            {
                // vertical read
                read_vertical_create(length, row, column, page, notebook, output);
            }
        }

        return output;
    }

    void Notebook::erase(int page, int row, int column, Direction dir, int length)
    {
        if (column > COL_LIMIT)
        {
            throw std::runtime_error("nagetive values");
        }
        check_negative_values_write(page, row, column);
        if (length < 0)
        {
            throw std::runtime_error("nagetive values");
        }
        if (dir == Direction::Horizontal)
        {
            bound_column_check(length, column);
        }

        std::string string_to_delete;
        if (notebook.find(page) == notebook.end())
        {
            std::map<int, std::string> pageOfbook;
            string_to_delete = std::string(COL_AMOUNT, '_');
            pageOfbook.insert(std::pair<int, std::string>(row, string_to_delete));
            notebook.insert(std::pair<int, std::map<int, std::string>>(page, pageOfbook));
        }
        else
        {
            if (notebook.at(page).find(row) == notebook.at(page).end())
            {
                string_to_delete = std::string(COL_AMOUNT, '_');
                notebook.at(page).insert(std::pair<int, std::string>(row, string_to_delete));
            }
        }
        string_to_delete = notebook.at(page).at(row);

        if (dir == Direction::Horizontal)
        {
            for (unsigned int i = ((unsigned int)column); i < column + ((int)length); i++)
            {
                string_to_delete.at(i) = '~';
            }
            notebook.at(page).at(row) = string_to_delete;
        }
        if (dir == Direction::Vertical)
        {
            for (int i = 0; i < (int)length; i++)
            {
                erase(page, ((int)row + i), column, Direction::Horizontal, 1);
            }
        }
    }

    void Notebook::show(int page)
    {
        check_page_not_negative(page);
        if (notebook.find(page) == notebook.end())
        {
            std::cout << "empty page number " << page << std::endl;
            return;
        }

        std::map<int, std::string> rows = notebook.at(page);
        for (auto const &row : rows)
        {
            std::cout << row.second << std::endl;
        }
    }

    void negative_values_new_check(int const &page, int const &row, int const &column, int const &length, Direction dir)
    {
        if (page < 0 || row < 0 || column < 0 || length < 0 || column >= COL_AMOUNT)
        {
            throw std::runtime_error("nagetive values");
        }
    }
}
