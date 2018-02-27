# libjson
Simple cpp json library
    
Usage:    
	json::Parse p;
	json::Value* v = p.read(message);
	if( v == NULL )
	{
		fprintf(stderr,"Failed to read responce.\n");
		return;
	}
	json::Object* o = dynamic_cast<json::Object*>(v);
	if( o == NULL )
	{
		delete v;
		fprintf(stderr,"Invalid opbject.\n");
		return;
	}
