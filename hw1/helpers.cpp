void render_res(Connection* connection, string type, string filter) {
    stringstream buffer;
    buffer << setw(7) << left << type \
        << setw(22) << connection->local_addr + ":" + (connection->local_port ? to_string(connection->local_port) : "*" ) \
        << setw(22) << connection->remote_addr + ":" + (connection->remote_port ? to_string(connection->remote_port) : "*" );
    if ( connection-> pid ) {
        buffer << setw(22) << setw(0) << connection->pid << "/" << connection->program << endl;
    }
    else {
        buffer << "-" << endl;
    }
    
    // regex filter
    regex rgx(filter);
    if ( filter.size() != 0 ) {
        if ( regex_search(buffer.str(), rgx) == 0 ) {
            return;
        }        
    }
    cout << buffer.str();
    return;
}

void render_res_v6(ConnectionV6* connection, string type, string filter) {
    stringstream buffer;
    buffer << setw(7) << left << type \
        << setw(22) << connection->local_addr + ":" + (connection->local_port ? to_string(connection->local_port) : "*" ) \
        << setw(22) << connection->remote_addr + ":" + (connection->remote_port ? to_string(connection->remote_port) : "*" );
    if ( connection-> pid ) {
        buffer << setw(22) << setw(0) << connection->pid << "/" << connection->program << endl;
    }
    else {
        buffer << "-" << endl;
    }
    
    // regex filter
    regex rgx(filter);
    if ( filter.size() != 0 ) {
        if ( regex_search(buffer.str(), rgx) == 0 ) {
            return;
        }        
    }
    cout << buffer.str();
    return;
}