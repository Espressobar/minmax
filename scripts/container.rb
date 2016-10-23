require 'erb'
require 'fileutils'
require 'securerandom'

if(ARGV.length < 1)
    puts "container NAME [SCREEN]"
    exit
end

class String
    def snake_case
        return downcase if match(/\A[A-Z]+\z/)
        gsub(/([A-Z]+)([A-Z][a-z])/, '\1_\2').
        gsub(/([a-z])([A-Z])/, '\1_\2').
        downcase
    end
    def camel_case
        return self if self !~ /_/ && self =~ /[A-Z]+.*/
        split('_').map{|e| e.capitalize}.join
    end
end

name = ARGV.shift
screen = ARGV.empty? ? 'common' : "#{ARGV.shift}_screen" 

PATH = '../../../../somfy-ice/trunk/CODE/SOURCE/Application/GUI_CENTRAL_RT'
IDE_PATH = '../../../../somfy-ice/trunk/CODE/IDE/simulator/msvs'
WINDOWS_LINE_ENDINGS = "\n"
UNIX_LINE_ENDINGS = "\n"
#on windows/mingw file.write will it self translate \n to \n, on linux not
LINE_ENDINGS = RUBY_PLATFORM.match(/linux/) ? WINDOWS_LINE_ENDINGS : UNIX_LINE_ENDINGS

the_name = name.snake_case
TheName = name.camel_case
the_screen = screen.snake_case
TheScreen = screen.camel_case

puts "generating container"

container_template_hpp = ERB.new( File.read('templates/Container.hpp.template') )

FileUtils.mkdir_p("#{PATH}/gui/include/gui/#{the_screen}")

FileUtils.cd("#{PATH}/gui/include/gui/#{the_screen}") do 

    if(File.exists?("#{TheName}.hpp"))
        puts "already exists, bailing out."
        exit
    end

    File.open("#{TheName}.hpp", 'w') do |f|
        f.write(container_template_hpp.result)
    end

end

container_template_cpp = ERB.new( File.read('templates/Container.cpp.template') )

FileUtils.mkdir_p("#{PATH}/gui/src/#{the_screen}")

FileUtils.cd("#{PATH}/gui/src/#{the_screen}") do 

    File.open("#{TheName}.cpp", 'w') do |f|
        f.write(container_template_cpp.result)
    end

end

vcxproj_path = "#{IDE_PATH}/Ice-Central-RT.vcxproj"
vcxproj = File.read(vcxproj_path)

contents = "    <ClCompile Include=\"..\\..\\..\\SOURCE\\Application\\GUI_CENTRAL_RT\\gui\\src\\#{the_screen}\\#{TheName}.cpp\" />"
vcxproj = vcxproj.sub(/(<ItemGroup>.+?<ClCompile Include="..\\..\\..\\SOURCE\\Application\\GUI_CENTRAL_RT\\gui\\src\\common\\FrontendApplication.cpp" \/>)/m, "\\1\n#{contents}")

contents = "    <ClInclude Include=\"..\\..\\..\\SOURCE\\Application\\GUI_CENTRAL_RT\\gui\\include\\gui\\#{the_screen}\\#{TheName}.hpp\" />"
vcxproj = vcxproj.sub(/(<ItemGroup>.+?<ClInclude Include="..\\..\\..\\SOURCE\\Application\\GUI_CENTRAL_RT\\gui\\include\\gui\\common\\FrontendApplication.hpp" \/>)/m, "\\1\n#{contents}")

File.open(vcxproj_path, 'w') { |f| f.write(vcxproj) }


vcxproj_filters_path = "#{IDE_PATH}/Ice-Central-RT.vcxproj.filters"
vcxproj_filters = File.read(vcxproj_filters_path)

contents = "    <ClCompile Include=\"..\\..\\..\\SOURCE\\Application\\GUI_CENTRAL_RT\\gui\\src\\#{the_screen}\\#{TheName}.cpp\">\n      <Filter>Source Files\\GUI_CENTRAL_RT\\#{the_screen}</Filter>\n    </ClCompile>"
vcxproj_filters = vcxproj_filters.sub(/(<ClCompile Include="..\\..\\..\\SOURCE\\Application\\GUI_CENTRAL_RT\\gui\\src\\common\\FrontendApplication.cpp">\s+<Filter>Source Files\\GUI_CENTRAL_RT\\common<\/Filter>\s+<\/ClCompile>)/m, "\\1\n#{contents}")

contents = "    <ClInclude Include=\"..\\..\\..\\SOURCE\\Application\\GUI_CENTRAL_RT\\gui\\include\\gui\\#{the_screen}\\#{TheName}.hpp\">\n      <Filter>Header Files\\GUI_CENTRAL_RT\\#{the_screen}<\/Filter>\n    </ClInclude>"
vcxproj_filters = vcxproj_filters.sub(/(<ClInclude Include="..\\..\\..\\SOURCE\\Application\\GUI_CENTRAL_RT\\gui\\include\\gui\\common\\FrontendApplication.hpp">\s+<Filter>Header Files\\GUI_CENTRAL_RT\\common<\/Filter>\s+<\/ClInclude>)/m, "\\1\n#{contents}")

File.open(vcxproj_filters_path, 'w') { |f| f.write(vcxproj_filters) }
