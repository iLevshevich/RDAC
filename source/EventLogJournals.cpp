#include "stdafx.h"
#include "EventLogJournals.h"

void Utility::EventLogJournals::init( const std::string& options_ ){
	boost::char_separator< char > journals_separator( "|", 0, boost::keep_empty_tokens );
	boost::tokenizer<boost::char_separator< char > > journals_tokens( options_, journals_separator );
	for ( boost::tokenizer< boost::char_separator< char > >::iterator journals_tokens_iter = journals_tokens.begin();  
																	  journals_tokens_iter != journals_tokens.end(); 
																    ++journals_tokens_iter ){
		const std::string::size_type journals_position( journals_tokens_iter->find( "(" ) );
		if( journals_position == std::string::npos ){
			throw std::runtime_error( "Invalid arguments" );
		}
		const std::string journal_name( boost::algorithm::trim_copy( journals_tokens_iter->substr( 0, journals_position ) ) );
		std::map< std::string, std::set< std::string > >current_levels;
		{
			const std::string level( boost::algorithm::trim_copy( journals_tokens_iter->substr( ( journals_position + 1 ), ( journals_tokens_iter->length() - ( journals_position + 2 ) ) ) ) );

			boost::char_separator< char > level_separator( ",", 0, boost::keep_empty_tokens );
			boost::tokenizer<boost::char_separator< char > > level_tokens( level, level_separator );

			for ( boost::tokenizer< boost::char_separator< char > >::iterator level_tokens_iter = level_tokens.begin();  
																			  level_tokens_iter != level_tokens.end(); 
																			++level_tokens_iter ){
				const std::string::size_type level_position( level_tokens_iter->find( "[" ) );
				if( level_position == std::string::npos ){
					throw std::runtime_error( "Invalid arguments" );
				}
				const std::string level_name( boost::algorithm::trim_copy( level_tokens_iter->substr( 0, level_position ) ) );
				std::set< std::string >current_codes;
				{
					const std::string code( boost::algorithm::trim_copy( level_tokens_iter->substr( ( level_position + 1 ), ( level_tokens_iter->length() - ( level_position + 2 ) ) ) ) );

					boost::char_separator< char > code_separator( ";", 0, boost::keep_empty_tokens );
					boost::tokenizer<boost::char_separator< char > > code_tokens( code, code_separator );
			
					for ( boost::tokenizer< boost::char_separator< char > >::iterator code_tokens_iter = code_tokens.begin();  
																			          code_tokens_iter != code_tokens.end(); 
																			        ++code_tokens_iter ){
						const std::string code_name( boost::algorithm::trim_copy( *code_tokens_iter ) );
						if( code_name.empty() ){
							throw std::runtime_error( "Invalid arguments" );
						}
						current_codes.insert( code_name );
					}
				}
				current_levels[ level_name.c_str() ] = current_codes;
			}
		}
		journals_options[ journal_name.c_str() ] = current_levels;
	}
}

bool Utility::EventLogJournals::isExist( const std::string& journal_, 
									     const std::string& level_, 
									     const std::string& code_ ){
	const std::map< std::string, std::map< std::string, std::set< std::string > > >::const_iterator journal_positions( journals_options.find( journal_ ) );
	if( journal_positions == journals_options.end() ){
		return false;
	}

	const std::map< std::string, std::set< std::string > >::const_iterator level_positions( journal_positions->second.find( level_ ) );
	if( level_positions == journal_positions->second.end() ){
		return false;
	}

	const std::set< std::string >::const_iterator code_positions_all( level_positions->second.find( "All" ) );
	const std::set< std::string >::const_iterator code_positions( level_positions->second.find( code_ ) );
	if( code_positions_all == level_positions->second.end() && 
		code_positions == level_positions->second.end() ){
		return false;
	}

	return true;
}

std::vector< std::string > Utility::EventLogJournals::getJournals() const{
	std::vector< std::string > result;
	for(const auto &iter: journals_options){
		result.push_back( iter.first );	
	}
	return result;
}