#!/usr/bin/env ruby
################################################################################
# wth2c.rb
# WITH source to C source translator
# version 0.0.1
# Designed by Richard Marks <ccpsceo@gmail.com>
# (C) Copyright 2009, CCPS Solutions
# http://www.ccpssolutions.com
################################################################################

class WITHParser
	def initialize
		@source_code = Array.new
		@source_includes = Array.new
		@source_targets = Array.new
		@source_actions = Array.new
		@source_directives = Array.new
	end
	
	def readInSourceAndFilterJunkLines(filename)
		@source_code = Array.new
		File.new(filename, "r").each do |line|
			if !line.eql? "\n" then
				if ! /^#+/i.match(line) then
					@source_code << line
				end
			end
		end
	end
	
	def readDirectives
		@source_code.each do |line|
			if /^\%/i.match(line) then
				@source_directives << line
			end
		end
	end
	
	def readIncludes
		# this is the regular expression to handle damn near any case that we ever need for include directives
		# yeesh! That was fun to figure out!
		# /^%(\s)?USE(\s)+(([a-zA-Z0-9]|(\.)*|(\/)*|(\\)*)*)((\s)+AS(\s)+([a-zA-Z0-9])*)?;$/i
=begin

the following has an error
^%\s?USE\s+([a-zA-Z0-9]|\.*|\/*|\\*+)(\s+AS\s+[a-zA-Z0-9]+)?;$

This one looks like it works the best
^%\s?USE\s+(\S+)\s*(\s+AS\s+(\S+))?;$

=end
		
		@source_code.each do |line|
			match_re = /^%\s?USE\s+(\S+)\s*(\s+AS\s+(\S+))?;$/i
			if match_re.match(line) then
				#puts "Debug: Matched USE directive: #{line}"
				line.scan(match_re) do |m|
					if !m[2].nil? then
						# we have an alias for our library
						lib_alias = m[2]
						lib_name = m[0]
					else
						# no alias
						lib_name = m[0]
						lib_alias = nil
					end
					
					#puts "Using LIB(#{lib_name}) AS (#{!lib_alias.nil? ? lib_alias : lib_name})"
					@source_includes.push [lib_name, lib_alias]
				end
				#puts "End Debug"
				# @source_includes << line
			end
		end
	end
	
	def readTargets
	end
	
	def readActions
	end

	def readWithCommands
		
		r_wsp			= /[\x20\t]/
		r_tok			= /[A-Za-z0-9\_\.]\w*/
		r_str_literal	= /"(?:[^\\"]|\\.)*"/
		r_int_literal	= /\d+/
		r_real_literal	= /-?\d+(?:\.\d+)?f/
		
		r_var			= /\@?\$#{r_tok}/
		r_arg			= /#{r_wsp}+(?:#{r_tok}|#{r_var}|#{r_str_literal}|#{r_int_literal}|#{r_real_literal})/
		r_act			= /(#{r_tok})(#{r_arg}*);$/
		
		r_wth			= /^#{r_wsp}*WITH#{r_wsp}+(#{r_tok})#{r_wsp}+DO#{r_wsp}+#{r_act}/i

		@source_code.grep(r_wth) do |line|
			target, command, arguments = *$~.captures
			
			if arguments.nil? then
				# no args
				puts "\t#{target}->#{command}();"
			else
				# args
				arguments = arguments.strip.to_a
				
				puts "\t#{target}->#{command}(#{arguments.join(', ')});"
			end
		end

	end
	
	def debugOutputSource
		line_counter = 0
		@source_code.each do |line|
			puts "\tLine #{line_counter + 1}: #{line}"
			line_counter += 1
		end
		puts "*** End Source"
	end
	
	def debugOutputDirectives
		@source_directives.each do |item| puts item end
		puts "*** End Directives"
	end
	
	def debugOutputIncludes
		
		@source_includes.each do |item| 
			#puts item
			lib_alias = "Aliased under the #{item[1]} namespace."
			puts "\tDepends on file: #{item[0]}.wth #{item[1].nil? ? '' : lib_alias}" 
		end
		puts "*** End Includes"
	end
	
	def debugOutputTargets
		@source_targets.each do |item| puts item end
		puts "*** End Targets"
	end
	
	def debugOutputActions
		@source_actions.each do |item| puts item end
		puts "*** End Actions"
	end
	
end

if __FILE__ == $0 then
	parser = WITHParser.new
	parser.readInSourceAndFilterJunkLines(ARGV[0])
	#parser.debugOutputSource
	#parser.readDirectives
	#parser.debugOutputDirectives
	#parser.readIncludes
	#parser.debugOutputIncludes
	parser.readWithCommands
end

