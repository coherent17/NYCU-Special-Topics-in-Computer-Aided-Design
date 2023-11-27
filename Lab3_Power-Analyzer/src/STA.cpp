#include "STA.h"

// Constructor & Destructor
STA::STA():
    Design_Name(""),
    Num_Cells(0),
    Num_Nets(0)
{
    this->library = new Library();
}

STA::~STA(){
    delete library;
    for(const auto &cell: Cells){
        delete cell.second;
    }
    for(const auto &net : Nets){
        delete net.second;
    }
}

// Parser
void STA::Parse_Library(const char *library_filename){
    this->library_filename = library_filename;
    ifstream library_file(library_filename);
    string Cell_Name, Pin_Name, Table_Name, line;
    smatch match;
    while(getline(library_file, line)){
        // Remove the comment and the space in the string
        line = regex_replace(line, Comment_Space_Pattern, "");
        
        // Continue for only "}" or " " in the line
        if(line.length() <= 1) continue;

        // Extract index1 & index2 info
        if(regex_search(line, Index_1_Pattern)){
            size_t Start_Index = line.find("\"");
            size_t End_Index = line.rfind("\"");
            string Extract_Values = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            while(regex_search(Extract_Values, match, Float_Pattern)){
                library->Index_1.emplace_back(stod(match.str()));
                Extract_Values = match.suffix().str();
            }
        }
        else if(regex_search(line, Index_2_Pattern)){
            size_t Start_Index = line.find("\"");
            size_t End_Index = line.rfind("\"");
            string Extract_Values = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            while(regex_search(Extract_Values, match, Float_Pattern)){
                library->Index_2.emplace_back(stod(match.str()));
                Extract_Values = match.suffix().str();
            }
        }

        // Extract Cell info
        else if(regex_search(line, Cell_Pattern)){
            size_t Start_Index = line.find("(");
            size_t End_Index = line.rfind(")");
            Cell_Name = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            library->Cell_LUT_Map[Cell_Name] = new Cell_LUT(Cell_Name);
        }

        else if(regex_search(line, Pin_Pattern)){
            size_t Start_Index = line.find("(");
            size_t End_Index = line.rfind(")");
            Pin_Name = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
        }

        // Extract Pin Cap
        else if(regex_search(line, Cap_Pattern)){
            size_t Start_Index = line.find(":");
            size_t End_Index = line.rfind(";");
            double Capacitance = stod(line.substr(Start_Index + 1, End_Index - Start_Index - 1));
            library->Cell_LUT_Map[Cell_Name]->Pin_Cap[Pin_Name] = Capacitance;
        }

        // Extract the 2-Dim LUT
        else if(regex_search(line, Table_Name_Pattern)){
            size_t End_Index = line.rfind("(");
            Table_Name = line.substr(0, End_Index);
        }

        else if(regex_search(line, Table_Pattern)){
            size_t Start_Index = line.find("\"");
            size_t End_Index = line.rfind("\"");
            string Extract_Values = line.substr(Start_Index + 1, End_Index - Start_Index - 1);
            while(regex_search(Extract_Values, match, Float_Pattern)){
                double Delay = stod(match.str());
                library->Cell_LUT_Map[Cell_Name]->Look_Up_Tables[Table_Name].emplace_back(Delay);
                Extract_Values = match.suffix().str();
            }
        }
    }
    library_file.close();
}

void STA::Parse_Netlist(const char *netlist_filename){
    // Find the Design Name
    this->netlist_filename = netlist_filename;
    string filename(netlist_filename);
    size_t Start_Index = filename.find_last_of("/");
    size_t End_Index = filename.find_last_of(".");
    Design_Name = filename.substr(Start_Index + 1, End_Index - Start_Index - 1);
    ifstream netlist_file(netlist_filename);
    
    // concat all of the verilog code together
    string Verilog_Code, line;
    while(getline(netlist_file, line)){
        Verilog_Code += line;
    }
    netlist_file.close();
    size_t found = 0;
    while((found = Verilog_Code.find(';', found + 1)) != std::string::npos){
        Verilog_Code.insert(found + 1, "\n");
        found += 2; // Move past the inserted '\n' to avoid infinite loop
    }
    
    //Start to parse the Netlist
    Cell *cell;
    Net *net;
    Cell_Type cell_type;
    Net_Type net_type;
    smatch match;
    stringstream ss(Verilog_Code);
    while(getline(ss, line)){
        // Remove the extra space
        line = regex_replace(line, Multi_Space_Pattern, " ");

        // Skip empty line
        if(line.length() < 1) continue;

        // Extract the net info
        if(regex_search(line, match, Net_Type_Pattern)){
            // Find the net type
            if(match.str() == "input") net_type = input;
            else if(match.str() == "output") net_type = output;
            else if(match.str() == "wire") net_type = wire;
            else abort();
            line = match.suffix().str();

            // Extract the net name that belongs to this net type
            while(regex_search(line, match, Word_Pattern)){
                string net_name = match.str();
                net = new Net(net_name, net_type);
                Nets[net_name] = net;
                line = match.suffix().str();
            }
        }

        // Extract the cell info
        else if(regex_search(line, match, Cell_Type_Pattern)){
            if(match.str() == "NOR2X1") cell_type = NOR2X1;
            else if(match.str() == "NANDX1") cell_type = NANDX1;
            else if(match.str() == "INVX1") cell_type = INVX1;
            else abort();
            line = match.suffix().str();

            // Extract the cell name
            regex_search(line, match, Word_Pattern);
            string cell_name = match.str();
            cell = new Cell(cell_name, cell_type);
            Cells[cell_name] = cell;
            line = match.suffix().str();

            // Extract the pin connection
            while(regex_search(line, match, Word_Pattern)){
                string pin_name = match.str();
                line = match.suffix().str();
                regex_search(line, match, Word_Pattern);
                string pin_connect_net_name = match.str();
                line = match.suffix().str();
                if(pin_name == "ZN"){
                    Nets[pin_connect_net_name]->Input_Cells = make_pair(pin_name, cell);
                    cell->Output_Net = Nets[pin_connect_net_name];
                    if(cell->Output_Net->Type == output){
                        cell->Output_Loading = PRIMARY_OUTPUT_LOADING;
                        Primary_Output_Cells.emplace_back(cell);
                    }
                }
                else if(pin_name == "A1" || pin_name == "A2" || pin_name == "I"){
                    Nets[pin_connect_net_name]->Output_Cells.emplace_back(make_pair(pin_name, cell));
                    cell->Input_Nets.emplace_back(Nets[pin_connect_net_name]);
                }
                else{
                    abort();
                }
            }
        }
    }
    Num_Cells = Cells.size();
    Num_Nets = Nets.size();
}

void STA::Parse_Pattern(const char *pattern_filename){
    ifstream pattern_file(pattern_filename);
    string line;
    getline(pattern_file, line);
    string no_comma_line = regex_replace(line, Comma_Regex, "");
    stringstream input_seq_ss(no_comma_line);
    input_seq_ss >> line;   // Read Input
    while(input_seq_ss >> line){
        Input_Order.emplace_back(line);
    }

    while(getline(pattern_file, line)){
        if(line == ".end") break;

        unordered_map<string, int> pattern;
        stringstream ss(line);
        for(size_t i = 0; i < Input_Order.size(); i++){
            int logic_value;
            ss >> logic_value;
            pattern[Input_Order[i]] = logic_value;
        }

        assert(pattern.size() == Input_Order.size());
        Patterns.emplace_back(pattern);
    }
    pattern_file.close();
}


void STA::Print_Library(){
    cout << *library << endl;
}

void STA::Print_Netlist(){
    cout << "Design Name: " << Design_Name << endl;
    cout << "Num Cells: " << Num_Cells << endl;
    cout << "Num Nets: " << Num_Nets << endl;
    cout << "Cells: " << endl;
    for (const auto &cell : Cells) {
        cout << *(cell.second) << endl;
    }
    cout << "Nets: " << endl;
    for (const auto &net : Nets){
        cout << *(net.second) << endl;
    }
}

void STA::Print_Pattern(){
    cout << "Input Order: ";
    for(size_t i = 0; i < Input_Order.size(); i++){
        cout << Input_Order[i] << " ";
    }
    cout << endl;
    for(size_t i = 0; i < Patterns.size(); i++){
        cout << "Pattern " << i + 1 << ": ";
        for(size_t j = 0; j < Input_Order.size(); j++){
            cout << Patterns[i][Input_Order[j]] << " ";
        }
        cout << endl;
    }
}

// Step 1
void STA::Calculate_Output_Loading(){
    for(const auto &pair1 : Cells){
        Cell *cell = pair1.second;
        Net *Output_Net = cell->Output_Net;
        double Output_Loading = cell->Output_Loading;
        for(const auto & pair2: Output_Net->Output_Cells){
            string pin_name = pair2.first;
            Cell_Type type = pair2.second->Type;
            string cell_type_str;
            if(type == NANDX1)  cell_type_str = "NANDX1";
            else if(type == INVX1) cell_type_str = "INVX1";
            else if(type == NOR2X1) cell_type_str = "NOR2X1";
            else abort();
            Output_Loading += library->Cell_LUT_Map[cell_type_str]->Pin_Cap[pin_name];
        }
        cell->Output_Loading = Output_Loading;
    }
}

void STA::Dump_Output_Loading(){
    ofstream fout(string(STUDENT_ID) + "_" + Design_Name + "_load.txt");
    auto cell_cmp = [](const Cell *c1, const Cell *c2){
        return c1->Instance_Number < c2->Instance_Number;
    };
    vector<Cell *> Sorted_Cells;
    for(const auto &cell : Cells){
        Sorted_Cells.emplace_back(cell.second);
    }
    sort(Sorted_Cells.begin(), Sorted_Cells.end(), cell_cmp);
    for(const auto &cell: Sorted_Cells){
        fout << cell->Name << " " << fixed << setprecision(6) << cell->Output_Loading << endl;
    }
    fout.close();
}

// Step 2
void STA::Cell_Topological_Sort(){
    // Find the indegree of each cells
    vector<Cell *> Cells_Copy;
    for(const auto &c : Cells){
        Cell *cell = c.second;
        for(const auto &net : cell->Input_Nets){
            if(net->Type != input) (cell->In_Degree)++;
        }
        Cells_Copy.emplace_back(cell);
    }

    // Perform topological sort using BFS
    queue<Cell *> Process_Queue;
    for(size_t i = 0; i < Cells_Copy.size(); i++){
        if(Cells_Copy[i]->In_Degree == 0){
            Cells_Copy[i]->Input_Transition_Time = 0;   // Boundary condition
            Process_Queue.push(Cells_Copy[i]);
        }
    }

    while(!Process_Queue.empty()){
        Cell *curr_cell = Process_Queue.front();
        Process_Queue.pop();
        Cells_In_Topological_Order.emplace_back(curr_cell);
        Net *curr_cell_output_net = curr_cell->Output_Net;
        for(const auto &connection : curr_cell_output_net->Output_Cells){
            Cell *curr_cell_output_cell = connection.second;
            (curr_cell_output_cell->In_Degree)--;
            if(curr_cell_output_cell->In_Degree == 0) Process_Queue.push(curr_cell_output_cell);
        }
    }
    assert(Cells_In_Topological_Order.size() == Num_Cells);
}

void STA::Set_Cell_Input_Transition_Time(Cell *cell){
    double input_transition_time = 0;
    if(cell->Type == NOR2X1){
        assert(cell->Input_Nets.size() == 2);
        if(cell->Input_Nets[0]->Type == input && cell->Input_Nets[1]->Type == input){
            input_transition_time = 0;
            cell->Arrival_Time = 0;
        }
        else if(cell->Input_Nets[0]->Type == input && cell->Input_Nets[1]->Type != input){
            if(cell->Input_Nets[0]->Logic_Value == 1){
                input_transition_time = 0;
                cell->Arrival_Time = 0;
            }
            else if(cell->Input_Nets[1]->Logic_Value == 1){
                Cell *pre_cell1 = cell->Input_Nets[1]->Input_Cells.second;
                input_transition_time = pre_cell1->Output_Transition_Time;
                cell->Arrival_Time = pre_cell1->Arrival_Time + pre_cell1->Propagation_Delay;
            }
            else{
                Cell *pre_cell1 = cell->Input_Nets[1]->Input_Cells.second;
                input_transition_time = pre_cell1->Output_Transition_Time;
                cell->Arrival_Time = pre_cell1->Arrival_Time + pre_cell1->Propagation_Delay;
            }
        }
        else if(cell->Input_Nets[0]->Type != input && cell->Input_Nets[1]->Type == input){
            if(cell->Input_Nets[1]->Logic_Value == 1){
                input_transition_time = 0;
                cell->Arrival_Time = 0;
            }
            else if(cell->Input_Nets[0]->Logic_Value == 1){
                Cell *pre_cell0 = cell->Input_Nets[0]->Input_Cells.second;
                input_transition_time = pre_cell0->Output_Transition_Time;
                cell->Arrival_Time = pre_cell0->Arrival_Time + pre_cell0->Propagation_Delay;
            }
            else{
                Cell *pre_cell0 = cell->Input_Nets[0]->Input_Cells.second;
                input_transition_time = pre_cell0->Output_Transition_Time;
                cell->Arrival_Time = pre_cell0->Arrival_Time + pre_cell0->Propagation_Delay;
            }
        }
        else{
            Cell *pre_cell0 = cell->Input_Nets[0]->Input_Cells.second;
            Cell *pre_cell1 = cell->Input_Nets[1]->Input_Cells.second;
            assert(pre_cell0 != nullptr && pre_cell1 != nullptr);
            double pre_cell0_arrival_time = pre_cell0->Arrival_Time + pre_cell0->Propagation_Delay;
            double pre_cell1_arrival_time = pre_cell1->Arrival_Time + pre_cell1->Propagation_Delay;

            // 0 0
            if(cell->Input_Nets[0]->Logic_Value == 0 && cell->Input_Nets[1]->Logic_Value == 0){
                if(pre_cell0_arrival_time > pre_cell1_arrival_time){
                    input_transition_time = pre_cell0->Output_Transition_Time;
                    cell->Arrival_Time = pre_cell0_arrival_time;
                }
                else{
                    input_transition_time = pre_cell1->Output_Transition_Time;
                    cell->Arrival_Time = pre_cell1_arrival_time;
                }
            }
            // 0 1
            else if(cell->Input_Nets[0]->Logic_Value == 0 && cell->Input_Nets[1]->Logic_Value == 1){
                input_transition_time = pre_cell1->Output_Transition_Time;
                cell->Arrival_Time = pre_cell1->Arrival_Time + pre_cell1->Propagation_Delay;
            }
            // 1 0
            else if(cell->Input_Nets[0]->Logic_Value == 1 && cell->Input_Nets[1]->Logic_Value == 0){
                Cell *pre_cell0 = cell->Input_Nets[0]->Input_Cells.second;
                input_transition_time = pre_cell0->Output_Transition_Time;
            }
            // 1 1
            else if(cell->Input_Nets[0]->Logic_Value == 1 && cell->Input_Nets[1]->Logic_Value == 1){
                if(pre_cell0_arrival_time < pre_cell1_arrival_time){
                    input_transition_time = pre_cell0->Output_Transition_Time;
                    cell->Arrival_Time = pre_cell0_arrival_time;
                }
                else{
                    input_transition_time = pre_cell1->Output_Transition_Time;
                    cell->Arrival_Time = pre_cell1_arrival_time;
                }
            }
            else abort();
        }
    }
    else if(cell->Type == NANDX1){
        assert(cell->Input_Nets.size() == 2);
        if(cell->Input_Nets[0]->Type == input && cell->Input_Nets[1]->Type == input){
            input_transition_time = 0;
            cell->Arrival_Time = 0;
        }
        else if(cell->Input_Nets[0]->Type == input && cell->Input_Nets[1]->Type != input){
            if(cell->Input_Nets[0]->Logic_Value == 0){
                input_transition_time = 0;
                cell->Arrival_Time = 0;
            }
            else if(cell->Input_Nets[1]->Logic_Value == 0){
                Cell *pre_cell1 = cell->Input_Nets[1]->Input_Cells.second;
                input_transition_time = pre_cell1->Output_Transition_Time;
                cell->Arrival_Time = pre_cell1->Arrival_Time + pre_cell1->Propagation_Delay;
            }
            else{
                Cell *pre_cell1 = cell->Input_Nets[1]->Input_Cells.second;
                input_transition_time = pre_cell1->Output_Transition_Time;
                cell->Arrival_Time = pre_cell1->Arrival_Time + pre_cell1->Propagation_Delay;
            }
        }
        else if(cell->Input_Nets[0]->Type != input && cell->Input_Nets[1]->Type == input){
            if(cell->Input_Nets[1]->Logic_Value == 0){
                input_transition_time = 0;
                cell->Arrival_Time = 0;
            }
            else if(cell->Input_Nets[0]->Logic_Value == 0){
                Cell *pre_cell0 = cell->Input_Nets[0]->Input_Cells.second;
                input_transition_time = pre_cell0->Output_Transition_Time;
                cell->Arrival_Time = pre_cell0->Arrival_Time + pre_cell0->Propagation_Delay;
            }
            else{
                Cell *pre_cell0 = cell->Input_Nets[0]->Input_Cells.second;
                input_transition_time = pre_cell0->Output_Transition_Time;
                cell->Arrival_Time = pre_cell0->Arrival_Time + pre_cell0->Propagation_Delay;
            }
        }
        else{
            Cell *pre_cell0 = cell->Input_Nets[0]->Input_Cells.second;
            Cell *pre_cell1 = cell->Input_Nets[1]->Input_Cells.second;
            assert(pre_cell0 != nullptr && pre_cell1 != nullptr);
            double pre_cell0_arrival_time = pre_cell0->Arrival_Time + pre_cell0->Propagation_Delay;
            double pre_cell1_arrival_time = pre_cell1->Arrival_Time + pre_cell1->Propagation_Delay;

            // 0 0
            if(cell->Input_Nets[0]->Logic_Value == 0 && cell->Input_Nets[1]->Logic_Value == 0){
                if(pre_cell0_arrival_time < pre_cell1_arrival_time){
                    input_transition_time = pre_cell0->Output_Transition_Time;
                    cell->Arrival_Time = pre_cell0_arrival_time;
                }
                else{
                    input_transition_time = pre_cell1->Output_Transition_Time;
                    cell->Arrival_Time = pre_cell1_arrival_time;
                }
            }
            // 0 1
            else if(cell->Input_Nets[0]->Logic_Value == 0 && cell->Input_Nets[1]->Logic_Value == 1){
                Cell *pre_cell0 = cell->Input_Nets[0]->Input_Cells.second;
                input_transition_time = pre_cell0->Output_Transition_Time;
            }
            // 1 0
            else if(cell->Input_Nets[0]->Logic_Value == 1 && cell->Input_Nets[1]->Logic_Value == 0){
                input_transition_time = pre_cell1->Output_Transition_Time;
                cell->Arrival_Time = pre_cell1->Arrival_Time + pre_cell1->Propagation_Delay;
            }
            // 1 1
            else if(cell->Input_Nets[0]->Logic_Value == 1 && cell->Input_Nets[1]->Logic_Value == 1){
                if(pre_cell0_arrival_time > pre_cell1_arrival_time){
                    input_transition_time = pre_cell0->Output_Transition_Time;
                    cell->Arrival_Time = pre_cell0_arrival_time;
                }
                else{
                    input_transition_time = pre_cell1->Output_Transition_Time;
                    cell->Arrival_Time = pre_cell1_arrival_time;
                }
            }
            else abort();
        }
    }
    else if(cell->Type == INVX1){
        assert(cell->Input_Nets.size() == 1);
        if(cell->Input_Nets[0]->Type == input){
            input_transition_time = 0;
            cell->Arrival_Time = 0;
        }
        else{
            Cell *pre_cell = cell->Input_Nets[0]->Input_Cells.second;
            assert(pre_cell != nullptr);
            input_transition_time = pre_cell->Output_Transition_Time;
            cell->Arrival_Time = pre_cell->Arrival_Time + pre_cell->Propagation_Delay;
        }
    }
    else abort();
    cell->Input_Transition_Time = input_transition_time;
}

double STA::Table_Interpolation(double C_req, double S_req, double C1, double C2, double S1, double S2, double P0, double P1, double P2, double P3){
    double A = P0 + ((P2 - P0) / (S2 - S1)) * (S_req - S1);
    double B = P3 + ((P1 - P3) / (S2 - S1)) * (S_req - S1);
    double P = A + ((B - A) / (C2 - C1)) * (C_req - C1);
    return P;
}

double STA::Table_Look_Up(Cell *cell, const string &Table_Name){
    string cell_type_str;
    if(cell->Type == NOR2X1) cell_type_str = "NOR2X1";
    else if(cell->Type == INVX1) cell_type_str = "INVX1";
    else if(cell->Type == NANDX1) cell_type_str = "NANDX1";
    else abort();

    size_t index_1_idx;
    size_t index_2_idx;
    for(index_1_idx = 0; index_1_idx < library->Index_1.size(); index_1_idx++){
        if(cell->Output_Loading < library->Index_1[index_1_idx]) break;
    }

    for(index_2_idx = 0; index_2_idx < library->Index_2.size(); index_2_idx++){
        if(cell->Input_Transition_Time < library->Index_2[index_2_idx]) break;
    }

    // find the require number for table interpolation
    const vector<double> Table = library->Get_Cell_Table(cell_type_str, Table_Name);
    assert(Table.size() == LUT_ROW_SIZE * LUT_COL_SIZE);

    // Out of the table range => extrapolation
    if(index_1_idx == 0) index_1_idx = 1;
    if(index_2_idx == 0) index_2_idx = 1;
    if(index_1_idx == LUT_COL_SIZE) index_1_idx = LUT_COL_SIZE - 1;
    if(index_2_idx == LUT_ROW_SIZE) index_2_idx = LUT_ROW_SIZE - 1;
    double C1 = library->Index_1[index_1_idx - 1];
    double C2 = library->Index_1[index_1_idx];
    double S1 = library->Index_2[index_2_idx - 1];
    double S2 = library->Index_2[index_2_idx];
    double P0 = Table[(index_1_idx - 1) + LUT_COL_SIZE * (index_2_idx - 1)];
    double P1 = Table[(index_1_idx) + LUT_COL_SIZE * (index_2_idx)];
    double P2 = Table[(index_1_idx - 1) + LUT_COL_SIZE * (index_2_idx)];
    double P3 = Table[(index_1_idx) + LUT_COL_SIZE * (index_2_idx - 1)];
    
    return Table_Interpolation(cell->Output_Loading, cell->Input_Transition_Time, C1, C2, S1, S2, P0, P1, P2, P3);
}

void STA::Calculate_Cell_Delay(){
    Cell_Topological_Sort();
    ofstream fout(string(STUDENT_ID) + "_" + Design_Name + "_gate_info.txt");
    auto cell_cmp = [](const Cell *c1, const Cell *c2){
        return c1->Instance_Number < c2->Instance_Number;
    };
    for(size_t i = 0; i < Patterns.size(); i++){
        // Reset all net logic value
        for(const auto &pair : Nets){
            pair.second->Logic_Value = UNSET;
        }

        // Set init pattern
        for(const auto &pair : Patterns[i]){
            cout << pair.first << " " << pair.second << endl;
            Nets[pair.first]->Logic_Value = pair.second;
        }

        for(const auto &cell : Cells_In_Topological_Order){
            cout << cell->Name << endl;
            if(cell->Type == NOR2X1){
                assert(cell->Input_Nets.size() == 2);
                assert(cell->Input_Nets[0]->Logic_Value != UNSET);
                assert(cell->Input_Nets[1]->Logic_Value != UNSET);
                cell->Output_Net->Logic_Value = !((bool)cell->Input_Nets[0]->Logic_Value || (bool)cell->Input_Nets[1]->Logic_Value);
                cout << cell->Output_Net->Logic_Value << endl;
            }
            else if(cell->Type == INVX1){
                assert(cell->Input_Nets.size() == 1);
                assert(cell->Input_Nets[0]->Logic_Value != UNSET);
                cell->Output_Net->Logic_Value = !(bool)cell->Input_Nets[0]->Logic_Value;
                cout << cell->Output_Net->Logic_Value << endl;
            }
            else if(cell->Type == NANDX1){
                assert(cell->Input_Nets.size() == 2);
                assert(cell->Input_Nets[0]->Logic_Value != UNSET);
                assert(cell->Input_Nets[1]->Logic_Value != UNSET);
                cell->Output_Net->Logic_Value = !((bool)cell->Input_Nets[0]->Logic_Value && (bool)cell->Input_Nets[1]->Logic_Value);
                cout << cell->Output_Net->Logic_Value << endl;
            }
            else abort();
            Set_Cell_Input_Transition_Time(cell);
            if(cell->Output_Net->Logic_Value == 0){
                double cell_falling_time = Table_Look_Up(cell, "cell_fall");
                double output_falling_time = Table_Look_Up(cell, "fall_transition");
                cell->Propagation_Delay = cell_falling_time;
                cell->Output_Transition_Time = output_falling_time;
            }
            else if(cell->Output_Net->Logic_Value == 1){
                double cell_rising_time = Table_Look_Up(cell, "cell_rise");
                double output_rising_time = Table_Look_Up(cell, "rise_transition");
                cell->Propagation_Delay = cell_rising_time;
                cell->Output_Transition_Time = output_rising_time;
            }
            else abort();
        }
        vector<Cell *> Sorted_Cells;
        for(const auto &cell : Cells){
            Sorted_Cells.emplace_back(cell.second);
        }
        sort(Sorted_Cells.begin(), Sorted_Cells.end(), cell_cmp);
        for(const auto &cell: Sorted_Cells){
            fout << cell->Name << " " << cell->Output_Net->Logic_Value << " " << fixed << setprecision(6) << cell->Propagation_Delay << " " << cell->Output_Transition_Time << endl;
        }
        fout << endl;

    }
    fout.close();
}