#include "gcode_parser.h"

gcode_parser::gcode_parser()
{

}

gcode_parser::GCODE gcode_parser::process_gcode(std::string filename){

    GCODE gc;
    BLOCK b;
    std::vector<BLOCK> blkvec;
    std::vector<std::string> strvec=read_file(filename);
    std::string comment;

    for(unsigned int i=0; i<strvec.size(); i++){

        FILTER f;
        f.brutoline=strvec[i];
        f=filter_line_comments(f,0);
        f=parse_a_line_into_chunks(f,0);
        f=parse_chunck_into_id_and_value(f,0);

        //! At this stage we have full a gcode line info parsed.
        b=chunck_id_value_to_block(f,b,0);

        //! Zero IJK values and clear comments.
        b=zero_some_block_values(b);
        blkvec.push_back(b);
    }

    print_results(blkvec,0);

    gc.blockvec=blkvec;
    //! gc.blockstringvec=strvec;
    //! gc.fullblockstring=get_fulltext(strvec);

    blkvec.clear();
    strvec.clear();
    return gc;
}

void gcode_parser::print_results(std::vector<BLOCK> blkvec, bool debug){
    for(unsigned int i=0; i<blkvec.size(); i++){
        if(debug){
            BLOCK b=blkvec[i];
            std::cout<<std::fixed<<"BLOCK i:"<<i<<std::endl;
            std::cout<<std::fixed<<"G:"<<b.G<<std::endl;
            std::cout<<std::fixed<<"X:"<<b.X<<std::endl;
            std::cout<<std::fixed<<"Y:"<<b.Y<<std::endl;
            std::cout<<std::fixed<<"Z:"<<b.Z<<std::endl;
            std::cout<<std::fixed<<"A:"<<b.A<<std::endl;
            std::cout<<std::fixed<<"B:"<<b.B<<std::endl;
            std::cout<<std::fixed<<"C:"<<b.C<<std::endl;
            std::cout<<std::fixed<<"U:"<<b.U<<std::endl;
            std::cout<<std::fixed<<"V:"<<b.V<<std::endl;
            std::cout<<std::fixed<<"W:"<<b.W<<std::endl;
            std::cout<<std::fixed<<"I:"<<b.I<<std::endl;
            std::cout<<std::fixed<<"J:"<<b.J<<std::endl;
            std::cout<<std::fixed<<"K:"<<b.K<<std::endl;
            std::cout<<std::fixed<<"M:"<<b.M<<std::endl;
            std::cout<<std::fixed<<"P:"<<b.P<<std::endl;
            std::cout<<std::fixed<<"S:"<<b.S<<std::endl;
            std::cout<<std::fixed<<"F:"<<b.F<<std::endl;
            std::cout<<std::fixed<<"comment:"<<b.comment<<std::endl;
        }
    }
}

std::string gcode_parser::get_fulltext(std::vector<std::string> strvec){
    std::string totalstr;
    for(unsigned int i=0; i<strvec.size(); i++){
        totalstr+=strvec[i]+"\n";
    }
    return totalstr;
}

gcode_parser::BLOCK gcode_parser::chunck_id_value_to_block(FILTER f, BLOCK b, bool debug){

    b.comment=f.comment;

    for(unsigned int i=0; i<f.chunkvec_id_value.size(); i++){

        std::string id=f.chunkvec_id_value[i].first;
        double val=f.chunkvec_id_value[i].second;

        if(id=="G" || id=="g"){
            b.G=val;
        }
        if(id=="X" || id=="x"){
            b.X=val;
        }
        if(id=="Y" || id=="y"){
            b.Y=val;
        }
        if(id=="Z" || id=="z"){
            b.Z=val;
        }
        if(id=="A" || id=="a"){
            b.A=val;
        }
        if(id=="B" || id=="b"){
            b.B=val;
        }
        if(id=="C" || id=="c"){
            b.C=val;
        }
        if(id=="U" || id=="u"){
            b.U=val;
        }
        if(id=="V" || id=="v"){
            b.V=val;
        }
        if(id=="W" || id=="w"){
            b.W=val;
        }
        if(id=="I" || id=="i"){
            b.I=val;
        }
        if(id=="J" || id=="j"){
            b.J=val;
        }
        if(id=="K" || id=="k"){
            b.K=val;
        }
        if(id=="M" || id=="m"){
            b.M=val;
        }
        if(id=="P" || id=="p"){
            b.P=val;
        }
        if(id=="S" || id=="s"){
            b.S=val;
        }
        if(id=="F" || id=="f"){
            b.F=val;
        }
        if(debug){
            std::cout<<std::fixed<<"A CHUNK"<<std::endl;
            std::cout<<std::fixed<<"G:"<<b.G<<std::endl;
            std::cout<<std::fixed<<"X:"<<b.X<<std::endl;
            std::cout<<std::fixed<<"Y:"<<b.Y<<std::endl;
            std::cout<<std::fixed<<"Z:"<<b.Z<<std::endl;
            std::cout<<std::fixed<<"A:"<<b.A<<std::endl;
            std::cout<<std::fixed<<"B:"<<b.B<<std::endl;
            std::cout<<std::fixed<<"C:"<<b.C<<std::endl;
            std::cout<<std::fixed<<"U:"<<b.U<<std::endl;
            std::cout<<std::fixed<<"V:"<<b.V<<std::endl;
            std::cout<<std::fixed<<"W:"<<b.W<<std::endl;
            std::cout<<std::fixed<<"I:"<<b.I<<std::endl;
            std::cout<<std::fixed<<"J:"<<b.J<<std::endl;
            std::cout<<std::fixed<<"K:"<<b.K<<std::endl;
            std::cout<<std::fixed<<"M:"<<b.M<<std::endl;
            std::cout<<std::fixed<<"P:"<<b.P<<std::endl;
            std::cout<<std::fixed<<"S:"<<b.S<<std::endl;
            std::cout<<std::fixed<<"F:"<<b.F<<std::endl;
            std::cout<<std::fixed<<"Comment:"<<b.comment<<std::endl;
        }
    }
    return b;
}

gcode_parser::FILTER gcode_parser::parse_chunck_into_id_and_value(FILTER f, bool debug){

    if(debug){std::cout<<"Funtion:parse_chunck_into_id_and_value"<<std::endl;}
    f.chunkvec_id_value.resize(f.chunkvec.size());
    for(unsigned int i=0; i<f.chunkvec.size(); i++){
        f.chunkvec_id_value[i].first=f.chunkvec[i][0];
        std::string val=f.chunkvec[i];
        val.erase(0,1);
        //! Avoid a empty string, can be caused by a typo.
        if(val.size()==0){
            val="0";
        }
        f.chunkvec_id_value[i].second=std::stod(val);
        if(debug){std::cout<<"id:"<<f.chunkvec_id_value[i].first<< " value:"<<f.chunkvec_id_value[i].second<<std::endl;}
    }
    if(debug){std::cout<<""<<std::endl;}
    return f;
}

gcode_parser::FILTER gcode_parser::filter_line_comments(FILTER f, bool debug){

    bool comment=0;
    for(unsigned int i=0; i<f.brutoline.size(); i++){
        //! If a comment is spot, the rest of the line is comment.
        if(f.brutoline[i]==';' || f.brutoline[i]=='#' || f.brutoline[i]=='(' || f.brutoline[i]=='/'){
            comment=true;
        }
        //! In case of a typo. No id value was given, thread as comment.
        if(f.brutoline.size()==1 || f.brutoline.size()==0){
            comment=true;
        }

        if(!comment){
            f.nettoline+=f.brutoline[i];
        }
        if(comment){
            f.comment+=f.brutoline[i];
        }
    }
    if(debug){std::cout<<"Funtion:filter_line_comments"<<std::endl;}
    if(debug){std::cout<<"brutoline:"<<f.brutoline<<std::endl;}
    if(debug){std::cout<<"nettoline:"<<f.nettoline<<std::endl;}
    if(debug){std::cout<<"comment:"<<f.comment<<std::endl;}
    if(debug){std::cout<<""<<std::endl;}
    return f;
}

gcode_parser::FILTER gcode_parser::parse_a_line_into_chunks(FILTER f, bool debug){

    if(debug){std::cout<<"Funtion:parse_a_line_into_chunks"<<std::endl;}
    std::string chunk;
    for(unsigned int i=0; i<f.nettoline.size(); i++){
        if(debug){std::cout<<"nettoline at i:"<<f.nettoline[i]<<std::endl;}
        //! Not a whitespace.
        if(f.nettoline[i]!=' '){
            chunk+=f.nettoline[i];
        }
        if(f.nettoline[i]==' ' || i==f.nettoline.size()-1){
            f.chunkvec.push_back(chunk);
            if(debug){std::cout<<"chunk:"<<chunk<<std::endl;}
            chunk.clear();
        }
    }
    if(debug){std::cout<<""<<std::endl;}
    return f;
}

gcode_parser::BLOCK gcode_parser::zero_some_block_values(BLOCK b){

    return b;
}

std::vector<std::string> gcode_parser::read_file(std::string filename){
    std::vector<std::string> strvec;
    std::fstream newfile;
    std::string str;
    newfile.open(filename, std::ios::in);
    if(newfile.is_open()){
        while(getline(newfile,str)){
            strvec.push_back(str);
        }
        newfile.close();
    }
    return strvec;
}
