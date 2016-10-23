require 'erb'
require 'fileutils'
require 'securerandom'
#require 'extlib'

if(ARGV.length < 1)
    puts "screen.rb NAME"
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

PATH = '../gui'
IDE_PATH = '../gui/simulator/msvs'
WINDOWS_LINE_ENDINGS = "\n"
UNIX_LINE_ENDINGS = "\n"
SUB = ARGV.shift
#on windows/mingw file.write will it self translate \n to \n, on linux not
LINE_ENDINGS = RUBY_PLATFORM.match(/linux/) ? WINDOWS_LINE_ENDINGS : UNIX_LINE_ENDINGS

the_name = name.snake_case
TheName = name.camel_case
#puts "Creating filters for #{the_name}" 
#camel_case = name.camel_case
#puts camel_case
#puts name.snake_case

puts "generating view for #{TheName}"

presenter_template_hpp = ERB.new( File.read('templates/Presenter.hpp.template') )
view_template_hpp = ERB.new( File.read('templates/View.hpp.template') )

FileUtils.mkdir_p("#{PATH}/gui/include/gui/#{the_name}_screen")

FileUtils.cd("#{PATH}/gui/include/gui/#{the_name}_screen") do 

    if(File.exists?("#{TheName}Presenter.hpp"))
        puts "already exists, bailing out."
        exit
    end

    File.open("#{TheName}Presenter.hpp", 'w') do |f|
        f.write(presenter_template_hpp.result)
    end
    File.open("#{TheName}View.hpp", 'w') do |f|
        f.write(view_template_hpp.result)
    end

end

presenter_template_cpp = ERB.new( File.read('templates/Presenter.cpp.template') )
view_template_cpp = ERB.new( File.read('templates/View.cpp.template') )

FileUtils.mkdir_p("#{PATH}/gui/src/#{the_name}_screen")

FileUtils.cd("#{PATH}/gui/src/#{the_name}_screen") do 

    File.open("#{TheName}Presenter.cpp", 'w') do |f|
        f.write(presenter_template_cpp.result)
    end
    File.open("#{TheName}View.cpp", 'w') do |f|
        f.write(view_template_cpp.result)
    end

end

frontend_application_hpp_path = "#{PATH}/gui/include/gui/common/FrontendApplication.hpp"
frontend_application_hpp = File.read(frontend_application_hpp_path)

frontend_application_hpp = frontend_application_hpp.sub(/(class FrontendApplication.*?public:)/m, "\\1\n    void goto#{TheName}();")
frontend_application_hpp = frontend_application_hpp.sub(/(class FrontendApplication.*?private:)/m, "\\1\n    void goto#{TheName}Impl();")

File.open(frontend_application_hpp_path, 'w') do |f|
    f.write(frontend_application_hpp)
end

frontend_application_cpp_path = "#{PATH}/gui/src/common/FrontendApplication.cpp"
frontend_application_cpp = File.read(frontend_application_cpp_path)

contents = "\#include <gui/#{the_name}_screen/#{TheName}View.hpp>\n\#include <gui/#{the_name}_screen/#{TheName}Presenter.hpp>"
frontend_application_cpp = frontend_application_cpp.sub(/(#include <gui\/common\/FrontendApplication\.hpp>)/m, "\\1\n#{contents}")

File.open(frontend_application_cpp_path, 'w') { |f| f.write(frontend_application_cpp) }

File.open(frontend_application_cpp_path, 'a') do |f|
    contents = <<-TEXT
void FrontendApplication::goto#{TheName}()
{ 
    transitionCallback = Callback< FrontendApplication >(this, &FrontendApplication::goto#{TheName}Impl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::goto#{TheName}Impl()
{
    makeTransition< #{TheName}View, #{TheName}Presenter, NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
    TEXT
    f.write(contents)
end

frontend_heap_hpp_path = "#{PATH}/gui/include/gui/common/FrontendHeap.hpp"
frontend_heap_hpp = File.read(frontend_heap_hpp_path)

contents = "\#include <gui/#{the_name}_screen/#{TheName}View.hpp>\n\#include <gui/#{the_name}_screen/#{TheName}Presenter.hpp>"
frontend_heap_hpp = frontend_heap_hpp.sub(/(#include <gui\/common\/FrontendApplication\.hpp>)/m, "\\1\n#{contents}")
frontend_heap_hpp = frontend_heap_hpp.sub( /([[:blank:]]*)meta::Nil([\s,>]+?)ViewTypes/m, "\\1meta::TypeList< #{TheName}View,\n\\1meta::Nil \\2> ViewTypes")
#frontend_heap_hpp = frontend_heap_hpp.sub( /^(\s+)touchgfx::meta::Nil([^;]+?)ViewTypes/m, "\\1touchgfx::meta::TypeList< #{TheName}View,\n\\1touchgfx::meta::Nil \\2> ViewTypes")
frontend_heap_hpp = frontend_heap_hpp.sub( /([[:blank:]]*)meta::Nil([\s,>]+?)PresenterTypes/m, "\\1meta::TypeList< #{TheName}Presenter,\n\\1meta::Nil \\2> PresenterTypes")
#frontend_heap_hpp = frontend_heap_hpp.sub( /^(\s+)touchgfx::meta::Nil([^;]+?)PresenterTypes/m, "\\1touchgfx::meta::TypeList< #{TheName}Presenter,\n\\1touchgfx::meta::Nil \\2> PresenterTypes")
 
puts frontend_heap_hpp
File.open(frontend_heap_hpp_path, 'w') { |f| f.write(frontend_heap_hpp) }

vcxproj_path = "#{IDE_PATH}/Application.vcxproj"
vcxproj = File.read(vcxproj_path)

contents = "    <ClCompile Include=\"..\\..\\gui\\src\\#{the_name}_screen\\#{TheName}Presenter.cpp\" />\n    <ClCompile Include=\"..\\..\\gui\\src\\#{the_name}_screen\\#{TheName}View.cpp\" />"
vcxproj = vcxproj.sub(/(<ItemGroup>.+?<ClCompile Include="..\\..\\gui\\src\\common\\FrontendApplication.cpp" \/>)/m, "\\1\n#{contents}")

contents = "    <ClInclude Include=\"..\\..\\gui\\include\\gui\\#{the_name}_screen\\#{TheName}Presenter.hpp\" />\n    <ClInclude Include=\"..\\..\\gui\\include\\gui\\#{the_name}_screen\\#{TheName}View.hpp\" />"
vcxproj = vcxproj.sub(/(<ItemGroup>.+?<ClInclude Include="..\\..\\gui\\include\\gui\\common\\FrontendApplication.hpp" \/>)/m, "\\1\n#{contents}")

File.open(vcxproj_path, 'w') { |f| f.write(vcxproj) }


vcxproj_filters_path = "#{IDE_PATH}/Application.vcxproj.filters"
vcxproj_filters = File.read(vcxproj_filters_path)

contents = "    <Filter Include=\"Source Files\\#{the_name}_screen\">\n      <UniqueIdentifier>{#{SecureRandom.uuid}}</UniqueIdentifier>\n    </Filter>\n    <Filter Include=\"Header Files\\#{the_name}_screen\">\n      <UniqueIdentifier>{#{SecureRandom.uuid}}</UniqueIdentifier>\n    </Filter>"
vcxproj_filters = vcxproj_filters.sub(/(<Filter Include="Source Files\\simulator">\s+<UniqueIdentifier>\{9DD49F7B-C117-4ABB-8692-FF6187CEA77A\}<\/UniqueIdentifier>\s+<\/Filter>)/m, "\\1\n#{contents}")
#puts vcxproj_filters

contents = "    <ClCompile Include=\"..\\..\\gui\\src\\#{the_name}_screen\\#{TheName}View.cpp\">\n      <Filter>Source Files\\#{the_name}_screen</Filter>\n    </ClCompile>\n    <ClCompile Include=\"..\\..\\gui\\src\\#{the_name}_screen\\#{TheName}Presenter.cpp\">\n      <Filter>Source Files\\#{the_name}_screen</Filter>\n    </ClCompile>"
vcxproj_filters = vcxproj_filters.sub(/(<ClCompile Include="..\\..\\gui\\src\\common\\FrontendApplication.cpp">\s+<Filter>Source Files\\common<\/Filter>\s+<\/ClCompile>)/m, "\\1\n#{contents}")
    
contents = "    <ClInclude Include=\"..\\..\\gui\\include\\gui\\#{the_name}_screen\\#{TheName}View.hpp\">\n      <Filter>Header Files\\#{the_name}_screen<\/Filter>\n    </ClInclude>\n    <ClInclude Include=\"..\\..\\gui\\include\\gui\\#{the_name}_screen\\#{TheName}Presenter.hpp\">\n      <Filter>Header Files\\#{the_name}_screen</Filter>\n    </ClInclude>"
vcxproj_filters = vcxproj_filters.sub(/(<ClInclude Include="..\\..\\gui\\include\\gui\\common\\FrontendApplication.hpp">\s+<Filter>Header Files\\common<\/Filter>\s+<\/ClInclude>)/m, "\\1\n#{contents}")

File.open(vcxproj_filters_path, 'w') { |f| f.write(vcxproj_filters) }

