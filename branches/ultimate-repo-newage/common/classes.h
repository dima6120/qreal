/** @file classes.h
 * 	@brief Классы, используемые для хранения элементов диаграмм в репозитории
 * */

#ifndef __CLASSES_H__
#define __CLASSES_H__

#include <QList>
#include <QString>
#include <QMap>
#include <QDebug>
#include <QPoint>

#include "defs.h"

class Link;
class Object;
class NodeOnDiagram;
class EdgeOnDiagram;

/** @class RepoData
 * 	@brief Класс, предоставляющий доступ ко всем объектам в репозитории.
 * */
class RepoData
{
public:
	RepoData();
	~RepoData();

	// called while creating new objects
	/** @brief Добавить объект */
	void addObject( int id, /**< Идентификатор */
					Object* obj /**< Объект */
					);
	/** @brief Добавить связь */
	void addLink( int id, /**< Идентификатор */
				Link* link /**< Связь */
				);
	/** @brief Удалить объект */
	void deleteObject( int id /**< Идентификатор */);
	/** @brief Удалить связь */
	void deleteLink( int id /**< Идентификатор */);

	/** @brief Получить объект
	 * 	@brief @return Объект
	 * */
	Object * getObject( int arg /**< Идентификатор */);
	/** @brief Получить связь
	 * 	@brief @return Связь
	 * */
	Link * getLink( int arg /**< Идентификатор */);

	/** @brief Получить число объектов */
	int getObjectsSize();
	/** @brief Получить число связей */
	int getLinksSize();

	// return list of objects of type `type'.
	// (mostly for object explorer needs)
	/** @brief Получить список идентификаторов объектов данного типа
	 * 	@brief @return Список идентификаторов объектов данного типа
	 * */
	QString getObjectsByType( int type /**< Идентификатор типа */);
	/** @brief Получить список идентификаторов связей данного типа
	 *	@brief @return Список идентификаторов связей данного типа
	 * */
	QString getLinksByType( int type /**< Идентификатор типа */);

	// returns some data, to be updated soon according to
	// suggestions made in trac wiki
	/** @brief Получить данные об объекте
	 *	@brief @return Данные об объекте
	 * */
	QString getLinkData( int type, /**< Идентификатор типа */
						int id /**< Идентификатор связи */
						);
	/** @brief Получить данные о связи
	 *	@brief @return Данные о связи
	 * */
	QString getObjectData( int type, /**< Идентификатор типа */
							int id /**< Идентификатор объекта */
							);
	/** @brief Получить корневой объект репозитория
	 *	@brief @return Корневой объект репозитория. (с) Капитан Очевидность.
	 * */
	Object* getRootObject() const;

private:
	/** @brief Объекты */
	QMap<int, Object*> objects;
	/** @brief Связи */
	QMap<int, Link*> links;
	/** @brief Корневой объект репозитория */
	Object *mRoot;
};

/** @class RepoElement
 * 	@brief Базовый класс для объектов и линков в репозитории
 * */
class RepoElement
{
public:
	// TODO: remove ID from constructor and generate it by repo server itself
	RepoElement(int id, int type);

	/** @brief Получить имя элемента
	 *	@brief @return Имя элемента
	 * */
	QString getName();
	/** @brief Установить имя элемента */
	void setName( QString arg /**< Имя */);

	/** @brief Получить описание элемента
	 *	@brief @return Описание элемента
	 * */
	QString getDescription();
	/** @brief Установить описание элемента */
	void setDescription( QString desc /**< Описание */);

	/** @brief Установить значние свойства */
	void setProperty( QString name, /**< Свойство */
					QString val /**< Значение */
					);
	/** @brief Получить значение свойства
	 *	@brief @return Значение свойства
	 * */
	QString getProperty( QString name /**< Свойство */);

	/** @brief Получить идентификатор элемента
	 *	@brief @return Идентификатор элемента
	 * */
	int getId();
	void setId(int id);
	/** @brief Получить тип элемента
	 *	@brief @return Тип элемента
	 * */
	int getType();

	/** @brief Получить список идентификаторов родителей */
	QString parentsToString();

	void addRef(int parent)
	{
		if (!parents.contains(parent))
			parents.append(parent);
	}

	void removeAllRefs()
	{
		parents.clear();
	}

	void removeRef(int parent)
	{
		if (parents.contains(parent)) {
			parents.removeAll(parent);
		}
	}

	int refCount()
	{	
		return parents.count();
	}

protected:
	/** @brief Идентификатор */
	int id;
	/** @brief Тип */
	int type;
	/** @brief Имя */
	QString name;
	/** @brief Описание */
	QString description;
	/** @brief Свойства и их значения */
	QMap<QString, QString> props;
	/** @brief Список родителей элемента */
	QList<int> parents;
};

/** @class Object
 * 	@brief Класс для хранения информации об объектах
 * */
class Object: public RepoElement
{
public:
	Object( int id, int type );

	/** @brief Получить число детей объекта
	 *	@brief @return Число детей объекта
	 * */
	int childrenCount();

	// add child to the children list, nothing more
	/** @brief Добавить дочерний элемент-объект */
	void addNodeChild( int id /**< Идентификатор элемента */);
	/** @brief Добавить дочерний элемент-связь */
	void addEdgeChild( int id /**< Идентификатор элемента */);
	/** @brief Убрать элемент из списка дочерних */
	void removeNodeChild( int id /**< Идентификатор элемента*/);
	/** @brief Убрать связь из списка дочерних */
	void removeEdgeChild( int id /**< Идентификатор связи*/);

	/** @brief Проверить, является ли указанный объект сыном данного */
	bool isParentOf(int id /**< Идентификатор элемента*/) const;

	/** @brief Изменить расположение дочернего элемента
	 * 	@brief @return Успешность выполнения операции
	 * */
	bool setChildPos( int id, /**< Идентификатор элемента */
					QString pos /**< Расположение элемента */
					);
	/** @brief Изменить конфигурацию дочернего элемента
	 * 	@brief @return Успешность выполнения операции
	 * */
	bool setChildConfiguration( int id, /**< Идентификатор элемента */
							QString conf /**< Конфигурация элемента */
							);
	/** @brief Изменить координаты дочернего элемента
	 * 	@brief @return Успешность выполнения операции
	 * */
	bool setChildCoord( int id, /**< Идентификатор элемента */
						QPoint p /**< Координаты элемента */
						);
	/** @brief Получить расположение дочернего элемента
	 * 	@brief @return Расположение дочернего элемента
	 * */
	QString getChildPos( int id /**< Идентификатор элемента */);
	/** @brief Получить конфигурацию дочернего элемента
	 * 	@brief @return Конфигурация дочернего элемента
	 * */
	QString getChildConfiguration( int id /**< Идентификатор элемента */);
	/** @brief Получить координаты дочернего элемента
	 * 	@brief @return Координаты дочернего элемента
	 * 	* */
	QPoint getChildCoord( int id /**< Идентификатор элемента */);

	// returns list of children entities' IDs
	/** @brief Получить список идентификаторов дочерних элементов */
	QString childrenToString();

	/** @brief Сериализовать объект в строку
	 *	@brief @return Сериализованный объект
	 * */
	QString toString();
	/** @brief Получить список идентификаторо входящих связей
	 *	@brief @return Список идентификаторов входящих связей
	 * */
	QString getIncomingLinks();
	/** @brief Получить список идентификаторов исходящих связей
	 *	@brief @return Список идентификаторов исходящих связей
	 * */
	QString getOutcomingLinks();

	/** @brief Добавить связь */
	void addLink( int id, /**< Идентификатор связи */
				int die /**< Направленность связи */
				);
	/** @brief Убрать связь */
	void removeLink( int id, /**< Идентификатор связи */
					int dir /**< Направленность связи */
					);

	void clearChildren()
	{
		nodeChildren.clear();
		edgeChildren.clear();
	}

	/** @brief Отладочный вывод */
	void print(){
		qDebug() << "incoming :" << incomingLinks;
		qDebug() << "outcoming:" << outcomingLinks;
		qDebug() << "props:" <<  props;
	}
private:
	/** @brief Список дочерних элементов */
	QMap<int, NodeOnDiagram> nodeChildren;
	/** @brief Список дочерних линков */
	QMap<int, EdgeOnDiagram> edgeChildren;
	/** @brief Список входящих связей */
	QList<int> incomingLinks;
	/** @brief Список исходящий связей */
	QList<int> outcomingLinks;
};

/* class for all edge entities
 */
/** @class Link
 * 	@brief Класс для хранения информации о связях
 * 	*/
class Link: public RepoElement
{
public:
	Link( int id, int type );

	/** @brief Сериализовать связь в строку
	 * 	@brief @return Строковое представление связи
	 * 	*/
	QString toString();

	/** @brief Получить список объектов, к которым присоединена связь
	 * 	@brief @return Список объектов, к которым присоединена связь
	 * 	*/
	QString getObjects();

	/** @brief Присоединить объект к началу связи */
	void addObjectTo( int id /**< Идентификатор объекта */);
	/** @brief Присоединить объект к концу связи */
	void addObjectFrom( int id /**< Идентификатор объекта */);

	/** @brief Отсоединить объект от начала связи */
	void removeObjectTo( int id /**< Идентификатор объекта */);
	/** @brief Отсоединить объект от конца связи */
	void removeObjectFrom( int id /**< Идентификатор объекта */);

	/** @brief Получить идентификатор объекта, присоединенного к началу связи
	 * 	@brief @return Идентификатор объекта, присоединенного к началу связи
	 * 	*/
	int getFrom();
	/** @brief Получить идентификатор объекта, присоединенного к концу связи
	 * 	@brief @return Идентификатор объекта, присоединенного к концу связи
	 * 	*/
	int getTo();

	/** @brief Отладочная печать */
	void print(){
		qDebug() << "to: :" << objectsTo;
		qDebug() << "from:" << objectsFrom;
		qDebug() << "props:" <<  props;
	}
private:
	/** @brief Получить список идентификаторов объектов, присоединенных к концу связи */
	QString getObjectsTo();
	/** @brief Получить список идентификаторов объектов, присоединенных к качалу связи */
	QString getObjectsFrom();

	/** @brief Объекты, присоединенные к концу связи */
	QList<int> objectsTo;
	/** @brief Объекты, присоединенные к началу связи */
	QList<int> objectsFrom;
};

/* types description
 */
/** @class TypeInfo
 * 	@brief Описание типа
 * */
class TypeInfo{
public:
	/** @brief Идентификатор */
	int id;
	/** @brief Число объектов данного типа */
	int count; // number of objects created ( for object explorer needs )
	/** @brief Имя */
	QString name;
	/** @brief Описание */
	QString qualifiedName; // `normal' description

	/** @brief Сериализовать описание типа в строку
	 * 	@brief @return Строковое представление информации о типе
	 * 	*/
	QString toString();
	/** @brief Десериализовать описание типа */
	void fromString( QString arg /**< Сериализованное значение описания типа */);
};

/** @class ElementOnDiagram
 * 	@brief Описание абстрактного элемента на диаграмме
 * */
class ElementOnDiagram {
public:
	/** @brief Конструктор */
	ElementOnDiagram() : id(0), configuration("") {}
	ElementOnDiagram( int mId /**< Иднетификатор */ ) : id(mId), configuration("") {}
	/** @brief Конструктор */
	ElementOnDiagram( int mId, /**< Иднетификатор */
						QString conf /**< Конфигурация */
						) : id(mId), configuration(conf) {}

	/** @brief Изменить конфигурацию элемента */
	void setConfiguration( QString str /**< Конфигурация */)
	{
		this->configuration = str;
	}

	/** @brief Получить конфигурацию объекта
	 *	@brief @return Конфигурация объекта
	 * */
	QString getConfiguration()
	{
		return configuration;
	}

	/**	@brief Получить идентификатор элемента
	 * 	@brief @return Идентификатор элемента
	 * */
	int getId()
	{
		return id;
	}

private:
	/** @brief Идентификатор */
	int id;
	/** @brief Конфигурация */
	QString configuration;
};

/** @class NodeOnDiagram
 * 	@brief Объект на диаграмме */
class NodeOnDiagram : public ElementOnDiagram {
public:
	/** @brief Конструктор */
	NodeOnDiagram() : ElementOnDiagram(), point(QPoint(0,0)){}
	/** @brief Конструктор */
	NodeOnDiagram( int id1 /**< Идентификатор */) : ElementOnDiagram(id1), point(QPoint(0,0))
	{
		setConfiguration(DEFAULT_NODE_CONFIGURATION);
	}
	/** @brief Конструктор */
	NodeOnDiagram( int id1, /**< Идентификатор */
					QString conf, /**< Конфигурация */
					QPoint p /**< Координаты */
					) : ElementOnDiagram(id1, conf), point(p) {}

	/** @brief Изменить координаты объекта */
	void setCoord( QPoint p /**< Координаты */)
	{
		this->point = p;
	}

	/** @brief Получить координаты объекта
	 * 	@brief @return Координаты объекта
	 * 	*/
	QPoint getCoord()
	{
		return point;
	}

private:
	/** @brief Координаты */
	QPoint point;
};

	/** @brief Связь на диаграмме */
class EdgeOnDiagram : public ElementOnDiagram {
public:
	/** @brief Конструктор */
	EdgeOnDiagram() : ElementOnDiagram(), position("") {}
	/** @brief Конструктор */
	EdgeOnDiagram( int id1 /**< Идентификатор */ ) : ElementOnDiagram(id1), position("")
	{
		setConfiguration(DEFAULT_EDGE_CONFIGURATION);
	}
	/** @brief Конструктор */
	EdgeOnDiagram( int id1, /**< Идентификатор */
					QString conf, /**< Конфигурация */
					QString pos /**< Расположение */
					) : ElementOnDiagram(id1, conf), position(pos) {}

	/** @brief Получить расположение связи
	 *	@brief @return Расположение связи
	 * */
	QString getPosition()
	{
		return position;
	}

	/** @brief Изменить расположение связи */
	void setPosition( QString pos /**< Расположение связи */)
	{
		this->position = pos;
	}
private:
	/** @brief Расположение связи */
	QString position;
};

#endif //__CLASSES_H__
