## Общая концепция построения программы на EO, полученной в ходе транспиляции с языка программирования C

Представленная структура стала возможной из-за использования начальной инициализации объектов, имитирующих переменные языка C.

Транспиляция осуществляется отдельно для каждой единицы компиляции, которая на выходе формирует два файла:

* файл с описанием всех глобальных объектов, к которым относятся абстрактные объекты, полученные при трансформации абстрактных типов данных, глобальных переменных, глобальных описаний функций;

* файл с описанием всех статических объектов, которые трансформируются из описаний статических переменных и функций, расположенных в глобальном пространстве, статических переменных, размещенных внутри функций.

Эти два файла являются базовой заготовкой для дальнейшей сборки после транспиляции всех единиц компиляции проекта. Сама сборка на текущий момент заключается в формировании общего файла на языке программирования EO. В нем формируется глобальный объект `global`, который содержит все объекты, полученные в результате компиляции абстрактных типов данных, внешних переменных, внешних функций, а также объектов, которые получены из файлов, описывающих статические объекты.

Количество статических объектов определяется количеством файлов со статическими артефактами. Размещение в едином объекте `global` всех данных позволяет без проблем обеспечить доступ как со стороны глобальных объектов к своим статическим данным, так и со стороных статических объектов к глобальным данным. Сборщик этого файла может в принципе быть отдельной программой, реализованной на любом удобном языке программирования.

При наличии в одной из единиц компиляции функции `main`, она преобразуется в соответствующий объект глобального пространства. А сразу за его описанием следует описание ее запуска. Функция может располагаться в любом месте глобального объекта.

В целом порядок сборки файла с глобальными объектами и статическими объектами несущественен.

Представленная схема обеспечивает полную автономность формирование программы на EO. Объект, запускающий приложение содержит только датаизацию глобального объекта. Он не меняется, оставаясь постоянным независимо от транспилируемого проекта.